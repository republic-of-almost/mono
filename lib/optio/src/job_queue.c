#include <job_queue.h>
#include <roa_array.h>
#include <config.h>
#include <mutex.h>
#include <optio/dispatcher.h>


/* ------------------------------------------------------ [ Job Lifetime ] -- */


void
optio_job_queue_create(struct optio_job_queue_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx != 0);
  FIBER_MEMZERO(ctx, sizeof(struct optio_job_queue_ctx));
  
  optio_mutex_create(&ctx->mutex);
  optio_mutex_lock(ctx->mutex);
  
  /* create arrays */
  optio_array_create(ctx->job_status,     FIBER_MAX_JOB_COUNT);
  optio_array_create(ctx->job_ids,        FIBER_MAX_JOB_COUNT);
  optio_array_create(ctx->jobs,           FIBER_MAX_JOB_COUNT);
  optio_array_create(ctx->job_batch_ids,  FIBER_MAX_JOB_COUNT);
  optio_array_create(ctx->batches,        FIBER_MAX_JOB_COUNT);
  optio_array_create(ctx->batch_ids,      FIBER_MAX_JOB_COUNT);
  
  optio_mutex_unlock(ctx->mutex);
}


void
optio_job_queue_destroy(struct optio_job_queue_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx != 0);
  
  optio_mutex_lock(ctx->mutex);

  /* destroy arrays */
  optio_array_destroy(ctx->job_status);
  optio_array_destroy(ctx->job_ids);
  optio_array_destroy(ctx->jobs);
  optio_array_destroy(ctx->job_batch_ids);
  optio_array_destroy(ctx->batches);
  optio_array_destroy(ctx->batch_ids);
  
  optio_mutex_unlock(ctx->mutex);
  optio_mutex_destroy(&ctx->mutex);
}


/* ---------------------------------------------------- [ Job Attributes ] -- */


unsigned
optio_job_queue_batch_size(struct optio_job_queue_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);
  
  optio_mutex_lock(ctx->mutex);
  
  const size_t size = optio_array_size(ctx->batch_ids);
  
  optio_mutex_unlock(ctx->mutex);
  
  return (unsigned)size;
}


unsigned
optio_job_queue_desc_size(struct optio_job_queue_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);
  
  optio_mutex_lock(ctx->mutex);
  
  const size_t size = optio_array_size(ctx->job_ids);
  
  optio_mutex_unlock(ctx->mutex);
  
  return (unsigned)size;
}


int
optio_job_queue_has_work(struct optio_job_queue_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);
  
  optio_mutex_lock(ctx->mutex);
  
  const size_t batch_size = optio_array_size(ctx->batches);
  const size_t job_size = optio_array_size(ctx->job_ids);
  
  optio_mutex_unlock(ctx->mutex);
  
  return (batch_size + job_size > 0 ? 1 : 0);
}


/* ------------------------------------------------------- [ Job Actions ] -- */


unsigned
optio_job_queue_next(
  struct optio_job_queue_ctx *ctx,
  void **out_func,
  void **out_arg)
{
  /* param check */
  FIBER_ASSERT(ctx != 0);
  FIBER_ASSERT(out_func);
  FIBER_ASSERT(out_arg);
  
  *out_func = 0;
  *out_arg = 0;

  int return_value = 0;

  optio_mutex_lock(ctx->mutex);
  
  int                       *job_status = ctx->job_status;
  const unsigned            *job_ids    = ctx->job_ids;
  const struct optio_job_desc *jobs       = ctx->jobs;
  
  const size_t job_count = optio_array_size(job_status);

  for(int i = 0; i < job_count; ++i)
  {
    const int curr_state = job_status[i];
  
    if(curr_state == optio_JOB_STATUS_PENDING)
    {
      return_value  = job_ids[i];
      job_status[i] = optio_JOB_STATUS_BUSY;
      
      *out_func = jobs[i].func;
      *out_arg  = jobs[i].arg;
      
      break;
    }
  }
  
  optio_mutex_unlock(ctx->mutex);
  
  return return_value;
}


unsigned
optio_job_queue_add_batch(
  struct optio_job_queue_ctx *ctx,
  struct optio_job_desc *desc,
  unsigned count)
{
  /* param check */
  FIBER_ASSERT(ctx != 0);
  FIBER_ASSERT(desc != 0);
  FIBER_ASSERT(count != 0);

  optio_mutex_lock(ctx->mutex);
  
  const int new_batch_id = ++ctx->batch_id_counter;
  
  /* new batch */
  {
    struct optio_job_batch batch;
    batch.total_jobs = count;
    batch.pending_jobs = count;
    
    optio_array_push(ctx->batches, batch);
    optio_array_push(ctx->batch_ids, new_batch_id);
  }
  
  /* add jobs */
  {
    for(int i = 0; i < count; ++i)
    {
      int new_job_id = ++ctx->job_id_counter;
    
      optio_array_push(ctx->jobs, desc[i]);
      optio_array_push(ctx->job_status, optio_JOB_STATUS_PENDING);
      optio_array_push(ctx->job_ids, new_job_id);
      optio_array_push(ctx->job_batch_ids, new_batch_id);
    }
  }
  
  optio_mutex_unlock(ctx->mutex);
  
  return new_batch_id;
}


unsigned
optio_job_queue_clear(
  struct optio_job_queue_ctx *ctx,
  unsigned job_id)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(job_id);
  
  int return_value = 0;
  int batch_id = 0;
  
  optio_mutex_lock(ctx->mutex);
  
  /* remove job */
  {
    size_t job_count = optio_array_size(ctx->job_ids);

    for(int i = 0; i < job_count; ++i)
    {
      if(ctx->job_ids[i] == job_id)
      {
        batch_id = ctx->job_batch_ids[i];
        
        FIBER_ASSERT(batch_id);
        
        optio_array_erase(ctx->job_ids, i);
        optio_array_erase(ctx->job_status, i);
        optio_array_erase(ctx->jobs, i);
        optio_array_erase(ctx->job_batch_ids, i);
        
        break;
      }
    }
  }
  
  /* shouldn't be able to get here without a batch_id */
  FIBER_ASSERT(batch_id);
  
  /* check should remove batch */
  if(batch_id)
  {
    const size_t batch_count = optio_array_size(ctx->batch_ids);
    
    for(int i = 0; i < batch_count; ++i)
    {
      if(ctx->batch_ids[i] == batch_id)
      {
        --ctx->batches[i].pending_jobs;
        
        if(ctx->batches[i].pending_jobs <= 0)
        {
          optio_array_erase(ctx->batch_ids, i);
          optio_array_erase(ctx->batches, i);
          
          return_value = batch_id;
          
          break;
        }
      }
    }
  }
  
  optio_mutex_unlock(ctx->mutex);
  
  return return_value;
}
