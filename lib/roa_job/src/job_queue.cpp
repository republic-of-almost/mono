#include <roa_job/dispatcher.h>
#include <counter.hpp>
#include <job_queue.hpp>
#include <roa_lib/array.h>
#include <config.hpp>
#include <roa_lib/mutex.h>
#include <roa_lib/assert.h>


/* ------------------------------------------------------ [ Job Lifetime ] -- */


void
roa_job_queue_create(struct roa_job_queue_ctx *ctx, unsigned queue_hint)
{
  /* param check */
  ROA_ASSERT(ctx != 0);
  memset(ctx, 0, sizeof(*ctx));
  
  ctx->mutex = roa_mutex_create();
  roa_mutex_lock(ctx->mutex);
  
  const unsigned count = queue_hint;
  
  /* create arrays */
  roa_array_create_with_capacity(ctx->job_status,     count);
  roa_array_create_with_capacity(ctx->job_ids,        count);
  roa_array_create_with_capacity(ctx->jobs,           count);
  roa_array_create_with_capacity(ctx->job_batch_ids,  count);
  roa_array_create_with_capacity(ctx->batches,        count);
  roa_array_create_with_capacity(ctx->batch_ids,      count);
  
  /* init to -1 */
  roa_array_create_with_capacity(ctx->counters, count);
  roa_array_resize(ctx->counters, count);
  
  for(unsigned i = 0; i < count; ++i)
  {
    const int value = -1;
    roa_counter_set(&ctx->counters[i], value, 0, -1);
  }
  
  roa_mutex_unlock(ctx->mutex);
}


void
roa_job_queue_destroy(struct roa_job_queue_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx != 0);
  
  roa_mutex_lock(ctx->mutex);

  /* destroy arrays */
  roa_array_destroy(ctx->job_status);
  roa_array_destroy(ctx->job_ids);
  roa_array_destroy(ctx->jobs);
  roa_array_destroy(ctx->job_batch_ids);
  roa_array_destroy(ctx->batches);
  roa_array_destroy(ctx->batch_ids);
  
  roa_mutex_unlock(ctx->mutex);
  roa_mutex_destroy(&ctx->mutex);
}


/* ---------------------------------------------------- [ Job Attributes ] -- */


unsigned
roa_job_queue_batch_size(struct roa_job_queue_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  
  roa_mutex_lock(ctx->mutex);
  
  const size_t size = roa_array_size(ctx->batch_ids);
  
  roa_mutex_unlock(ctx->mutex);
  
  return (unsigned)size;
}


unsigned
roa_job_queue_desc_size(struct roa_job_queue_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  
  roa_mutex_lock(ctx->mutex);
  
  const size_t size = roa_array_size(ctx->job_ids);
  
  roa_mutex_unlock(ctx->mutex);
  
  return (unsigned)size;
}


int
roa_job_queue_has_work(struct roa_job_queue_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  
  roa_mutex_lock(ctx->mutex);
  
  const size_t batch_size = roa_array_size(ctx->batches);
  const size_t job_size = roa_array_size(ctx->job_ids);
  
  roa_mutex_unlock(ctx->mutex);
  
  return (batch_size + job_size > 0 ? 1 : 0);
}


/* ------------------------------------------------------- [ Job Actions ] -- */


unsigned
roa_job_queue_next(
  struct roa_job_queue_ctx *ctx,
  int th_id,
  void **out_func,
  void **out_arg,
  ROA_BOOL *out_thread_locked)
{
  /* param check */
  ROA_ASSERT(ctx != 0);
  ROA_ASSERT(out_func);
  ROA_ASSERT(out_arg);
  
  *out_func = 0;
  *out_arg = 0;

  int return_value = 0;

  roa_mutex_lock(ctx->mutex);
  
  int                  *job_status = ctx->job_status;
  const unsigned       *job_ids    = ctx->job_ids;
  const struct roa_job *jobs       = ctx->jobs;
  
  const size_t job_count = roa_array_size(job_status);

  for(size_t i = 0; i < job_count; ++i)
  {
    const int curr_state = job_status[i];
  
    if(curr_state == roa_JOB_STATUS_PENDING)
    {
      const int calling_th = jobs[i].calling_th_id;

      if(calling_th == -1 || calling_th == th_id)
      {
        return_value  = job_ids[i];
        job_status[i] = roa_JOB_STATUS_BUSY;
      
        *out_func          = (void*)jobs[i].func;
        *out_arg           = (void*)jobs[i].arg;
        *out_thread_locked = calling_th == -1 ? ROA_FALSE : ROA_TRUE;
      
        break;
      }
    }
  }
  
  roa_mutex_unlock(ctx->mutex);
  
  return return_value;
}


unsigned
roa_job_queue_add_batch(
  struct roa_job_queue_ctx *ctx,
  const struct roa_job_desc *desc,
  unsigned count,
  int thread_id)
{
  /* param check */
  ROA_ASSERT(ctx != 0);
  ROA_ASSERT(desc != 0);
  ROA_ASSERT(count != 0);

  roa_mutex_lock(ctx->mutex);
  
  const int new_batch_id = ++ctx->batch_id_counter;
  
  /* new batch */
  {
    struct roa_job_batch batch;
    batch.total_jobs = count;
    batch.pending_jobs = count;
    batch.is_blocked = 0;
    batch.counter = NULL;

    /* find a counter */
    {
      const size_t counter_size = roa_array_size(ctx->counters);
      
      for(size_t i = 0; i < counter_size; ++i)
      {
        int value = roa_counter_value(&ctx->counters[i]);

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
        const size_t old_counter_size = roa_array_size(ctx->counters);
        const size_t new_counter_size = old_counter_size + 8;

        roa_array_resize(ctx->counters, new_counter_size);

        for(size_t i = old_counter_size; i < new_counter_size; ++i)
        {
          const int value = -1;
          roa_counter_set(&ctx->counters[i], value, 0, thread_id);
          ctx->counters[i].has_pending = 0;
          ctx->counters[i].batch_id = 0;
        }

        /*struct roa_counter new_counter;
        roa_counter_set(&new_counter, -1, -1);
        roa_array_push(ctx->counters, new_counter);*/

        //batch.counter = &(roa_array_back(ctx->counters));
      }
    }
    
    roa_counter_set(batch.counter, count, new_batch_id, thread_id);
    
    roa_array_push(ctx->batches, batch);
    roa_array_push(ctx->batch_ids, new_batch_id);
  }
  
  /* add jobs */
  {
    for(unsigned i = 0; i < count; ++i)
    {
      int new_job_id = ++ctx->job_id_counter;
     
      roa_job new_desc;
      new_desc.func = desc[i].func;
      new_desc.arg  = desc[i].arg;

      if (desc[i].keep_on_calling_thread == ROA_TRUE)
      {
        new_desc.calling_th_id = thread_id;
      }
      else
      {
        new_desc.calling_th_id = -1;
      }

      roa_array_push(ctx->jobs, new_desc);
      roa_array_push(ctx->job_status, roa_JOB_STATUS_PENDING);
      roa_array_push(ctx->job_ids, new_job_id);
      roa_array_push(ctx->job_batch_ids, new_batch_id);
    }
  }
  
  roa_mutex_unlock(ctx->mutex);
  
  return new_batch_id;
}


struct roa_counter *
roa_job_queue_batch_block(
  struct roa_job_queue_ctx *ctx,
  unsigned batch_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(batch_id);
  
  struct roa_counter *counter = NULL;
  
  roa_mutex_lock(ctx->mutex);
  
  size_t count = roa_array_size(ctx->batch_ids);
  
  for(size_t i = 0; i < count; ++i)
  {
    if(ctx->batch_ids[i] == batch_id)
    {
      struct roa_job_batch *batch = &ctx->batches[i];
      batch->is_blocked = 1;
      
      ROA_ASSERT(batch);
    
      counter = batch->counter;
    }
  }
  
  roa_mutex_unlock(ctx->mutex);
  
  return counter;
}


void
roa_job_queue_batch_unblock(
  struct roa_job_queue_ctx *ctx,
  struct roa_counter *counter)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(counter);
  
  roa_mutex_lock(ctx->mutex);
  
  /* check should remove batch */
  if(counter)
  {
    counter->has_pending = 0;
    unsigned batch_id = counter->batch_id;
  
    const size_t batch_count = roa_array_size(ctx->batch_ids);
    
    for(size_t i = 0; i < batch_count; ++i)
    {
      if(ctx->batch_ids[i] == batch_id)
      {
        struct roa_job_batch *batch = &ctx->batches[i];
      
        const int has_pending_jobs = batch->pending_jobs > 0 ? 1 : 0;
        const int has_pending_fiber = batch->counter->has_pending > 0 ? 1 : 0;
        
        if(has_pending_jobs == 0 && has_pending_fiber == 0)
        {
          roa_counter_set(batch->counter, -1, -1, -1);
          
          roa_array_erase(ctx->batch_ids, i);
          roa_array_erase(ctx->batches, i);
          
          break;
        }
      }
    }
  }
  
  roa_mutex_unlock(ctx->mutex);
}


unsigned
roa_job_queue_clear(
  struct roa_job_queue_ctx *ctx,
  unsigned job_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(job_id);
  
  int return_value = 0;
  unsigned batch_id = 0;
  
  roa_mutex_lock(ctx->mutex);
  
  /* remove job */
  {
    size_t job_count = roa_array_size(ctx->job_ids);

    for(size_t i = 0; i < job_count; ++i)
    {
      if(ctx->job_ids[i] == job_id)
      {
        batch_id = ctx->job_batch_ids[i];
        
        ROA_ASSERT(batch_id);
        
        roa_array_erase(ctx->job_ids, i);
        roa_array_erase(ctx->job_status, i);
        roa_array_erase(ctx->jobs, i);
        roa_array_erase(ctx->job_batch_ids, i);
        
        break;
      }
    }
  }
  
  /* shouldn't be able to get here without a batch_id */
  ROA_ASSERT(batch_id);
  
  /* check should remove batch */
  if(batch_id)
  {
    const size_t batch_count = roa_array_size(ctx->batch_ids);
    
    for(size_t i = 0; i < batch_count; ++i)
    {
      if(ctx->batch_ids[i] == batch_id)
      {
        struct roa_job_batch *batch = &ctx->batches[i];
      
        --batch->pending_jobs;
        
        roa_counter_decrement(batch->counter);
        
        const int has_pending_jobs = batch->pending_jobs > 0 ? 1 : 0;
        const int has_pending_fiber = batch->is_blocked > 0 ? 1 : 0;
        
        if(has_pending_jobs == 0 && has_pending_fiber == 0)
        {
          roa_array_erase(ctx->batch_ids, i);
          roa_array_erase(ctx->batches, i);

          if (!batch->is_blocked)
          {
            roa_counter_set(batch->counter, -1, -1, -1);
          }
          
          return_value = batch_id;
          
          break;
        }
      }
    }
  }
  
  roa_mutex_unlock(ctx->mutex);
  
  
  return return_value;
}

