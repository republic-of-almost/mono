#include <optio/dispatcher.h>
#include <counter.hpp>
#include <job_queue.hpp>
#include <roa_array.hpp>
#include <config.hpp>
#include <mutex.hpp>



/* ------------------------------------------------------ [ Job Lifetime ] -- */


void
optio_job_queue_create(struct optio_job_queue_ctx *ctx, unsigned queue_hint)
{
  /* param check */
  FIBER_ASSERT(ctx != 0);
  FIBER_MEMZERO(ctx, sizeof(struct optio_job_queue_ctx));
  
  optio_mutex_create(&ctx->mutex);
  optio_mutex_lock(ctx->mutex);
  
  const unsigned count = queue_hint > 0 ? queue_hint : FIBER_MAX_JOB_COUNT;
  
  /* create arrays */
  optio_array_create(ctx->job_status,     count);
  optio_array_create(ctx->job_ids,        count);
  optio_array_create(ctx->jobs,           count);
  optio_array_create(ctx->job_batch_ids,  count);
  optio_array_create(ctx->batches,        count);
  optio_array_create(ctx->batch_ids,      count);
  
  /* init to -1 */
  optio_array_create(ctx->counters, count);
  optio_array_resize(ctx->counters, count);
  
  for(unsigned i = 0; i < count; ++i)
  {
    const int value = -1;
    optio_counter_set(&ctx->counters[i], value, 0);
  }
  
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

  for(size_t i = 0; i < job_count; ++i)
  {
    const int curr_state = job_status[i];
  
    if(curr_state == optio_JOB_STATUS_PENDING)
    {
      return_value  = job_ids[i];
      job_status[i] = optio_JOB_STATUS_BUSY;
      
      *out_func = (void*)jobs[i].func;
      *out_arg  = (void*)jobs[i].arg;
      
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
    batch.is_blocked = 0;
    batch.counter = NULL;
    
    /* find a counter */
    {
      const size_t counter_size = optio_array_size(ctx->counters);
      
      for(size_t i = 0; i < counter_size; ++i)
      {
        int value = optio_counter_value(&ctx->counters[i]);
        
        if(value < 0)
        {
          batch.counter = &ctx->counters[i];
          batch.counter->has_pending = 0;
          
          break;
        }
      }
      
      /* create new counter if we ran out */
      if(batch.counter == NULL)
      {
        /* simple huristic */
        const size_t old_counter_size = optio_array_size(ctx->counters);
        const size_t new_counter_size = old_counter_size + 8;

        optio_array_resize(ctx->counters, new_counter_size);

        for(size_t i = old_counter_size; i < new_counter_size; ++i)
        {
          const int value = -1;
          optio_counter_set(&ctx->counters[i], value, 0);
          ctx->counters[i].has_pending = 0;
          ctx->counters[i].batch_id = 0;
        }

        //struct optio_counter new_counter;
        //optio_counter_set(&new_counter, -1, -1);
        
        //optio_array_push(ctx->counters, new_counter);
        
        //batch.counter = &(optio_array_back(ctx->counters));
      }
    }
    
    optio_counter_set(batch.counter, count, new_batch_id);
    
    optio_array_push(ctx->batches, batch);
    optio_array_push(ctx->batch_ids, new_batch_id);
  }
  
  /* add jobs */
  {
    for(unsigned i = 0; i < count; ++i)
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


struct optio_counter *
optio_job_queue_batch_block(
  struct optio_job_queue_ctx *ctx,
  unsigned batch_id)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(batch_id);
  
  struct optio_counter *counter = NULL;
  
  optio_mutex_lock(ctx->mutex);
  
  size_t count = optio_array_size(ctx->batch_ids);
  
  for(size_t i = 0; i < count; ++i)
  {
    if(ctx->batch_ids[i] == batch_id)
    {
      struct optio_job_batch *batch = &ctx->batches[i];
      batch->is_blocked = 1;
      
      FIBER_ASSERT(batch);
    
      counter = batch->counter;
    }
  }
  
  optio_mutex_unlock(ctx->mutex);
  
  return counter;
}


void
optio_job_queue_batch_unblock(
  struct optio_job_queue_ctx *ctx,
  struct optio_counter *counter)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(counter);
  
  optio_mutex_lock(ctx->mutex);
  
  /* check should remove batch */
  if(counter)
  {
    counter->has_pending = 0;
    unsigned batch_id = counter->batch_id;
  
    const size_t batch_count = optio_array_size(ctx->batch_ids);
    
    for(size_t i = 0; i < batch_count; ++i)
    {
      if(ctx->batch_ids[i] == batch_id)
      {
        struct optio_job_batch *batch = &ctx->batches[i];
      
        const int has_pending_jobs = batch->pending_jobs > 0 ? 1 : 0;
        const int has_pending_fiber = batch->counter->has_pending > 0 ? 1 : 0;
        
        if(has_pending_jobs == 0 && has_pending_fiber == 0)
        {
          optio_counter_set(batch->counter, -1, -1);
          
          optio_array_erase(ctx->batch_ids, i);
          optio_array_erase(ctx->batches, i);
          
          break;
        }
      }
    }
  }
  
  optio_mutex_unlock(ctx->mutex);
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
  unsigned batch_id = 0;
  
  optio_mutex_lock(ctx->mutex);
  
  /* remove job */
  {
    size_t job_count = optio_array_size(ctx->job_ids);

    for(size_t i = 0; i < job_count; ++i)
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
    
    for(size_t i = 0; i < batch_count; ++i)
    {
      if(ctx->batch_ids[i] == batch_id)
      {
        struct optio_job_batch *batch = &ctx->batches[i];
      
        --batch->pending_jobs;
        
        optio_counter_decrement(batch->counter);
        
        const int has_pending_jobs = batch->pending_jobs > 0 ? 1 : 0;
        const int has_pending_fiber = batch->is_blocked > 0 ? 1 : 0;
        
        if(has_pending_jobs == 0 && has_pending_fiber == 0)
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

