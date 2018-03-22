#include <roa_job/roa_job.h>
#include <thread_dispatch/thread_process.h>
#include <thread_dispatch/thread_local_storage.h>
#include <roa_lib/thread.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/array.h>
#include <roa_lib/signal.h>
#include <roa_lib/time.h>
#include <ctx/context.h>
#include <fiber/fiber.h>
#include <jobs/jobs.h>
#include <roa_lib/spin_lock.h>


void
fiber_process(void *arg)
{
  /* param check */
  ROA_ASSERT(arg);

  /* get ctx */
  struct roa_job_dispatcher_ctx *ctx = (struct roa_job_dispatcher_ctx*)arg;
  ROA_ASSERT(ctx);

  /* one time tls search */
  /*
    if we move to a model where fibers can wake up
    on different threads this needs to change
  */
  int th_index = job_internal_find_thread_index(ctx);
  struct thread_local_storage *tls = &ctx->tls[th_index];

  ROA_ASSERT(tls->home_fiber);

  while (1)
  {
    /* exec job */
    {
      struct job_internal job_data = tls->executing_fiber.desc;
      struct roa_job_desc desc = job_data.desc;

      roa_job_fn job_func = (roa_job_fn)desc.func;
      void *job_arg = desc.arg;

      job_func(ctx, job_arg);
    }

    /* decrement job counter */
    /* clear executing as its done */
    {
      /* decrement job count */
      {
        unsigned count = roa_array_size(tls->batch_ids);
        unsigned i;
        uint32_t batch_to_find = tls->executing_fiber.desc.batch_id;

        roa_spin_lock_aquire(&tls->job_lock);

        for (i = 0; i < count; ++i)
        {
          uint32_t batch_id = tls->batch_ids[i];

          if (batch_to_find == batch_id)
          {
            struct job_batch *batch = &tls->batches[i];
            batch->count -= 1;

            if (batch->count <= 0)
            {
              roa_array_erase(tls->batch_ids, i);
              roa_array_erase(tls->batches, i);
            }

            break;
          }
        }

        roa_spin_lock_release(&tls->job_lock);
      }
    }

    /* swtich back */
    {
      struct roa_fiber *worker = tls->executing_fiber.worker_fiber;
      ROA_ASSERT(worker);

      struct roa_fiber *home = tls->home_fiber;
      ROA_ASSERT(home);

      roa_fiber_switch(worker, home);
    }
  }
}

void*
thread_process(void *arg)
{
  /* param check */
  ROA_ASSERT(arg);

  /* get/set data setup */
  struct roa_job_dispatcher_ctx *ctx;
  roa_thread_id curr_th_id = roa_thread_get_current_id();
  {
    struct thread_arg *th_arg = (struct thread_arg*)arg;
    ROA_ASSERT(th_arg);
    *th_arg->roa_thread_id = curr_th_id;

    ctx = th_arg->ctx;
    roa_free(arg);
  }

  roa_thread_set_current_name("ROA_Job Thread");
  
  /* find self in thread pool */
  struct thread_local_storage *tls = ROA_NULL;
  int th_index = -1;
  {
    int count = ctx->thread_count;
    int i;

    for (i = 0; i < count; ++i)
    {
      roa_thread_id that_id = ctx->thread_ids[i];
      if (that_id == curr_th_id)
      {
        th_index = i;
        break;
      }
    }
    ROA_ASSERT(th_index > -1);

    tls = &ctx->tls[th_index];
    ROA_ASSERT(tls);

    tls->thread_status = THREAD_STATE_STARTING;
  }
  
  roa_fiber_create(&tls->home_fiber, ROA_NULL, ROA_NULL);
  
  /*
    first check pending fibers, if found switch to.
    then check pending jobs, if found create pending fiber.
    then try steal work from other threads, and create a pending job.
  */

  /* wait until signal */
  if(th_index > 0)
  {
    roa_signal_wait(ctx->signal_start, 1);
  }
  else
  {
    roa_signal_raise(ctx->signal_start);
  }


  while (1)
  {
    tls->thread_status = THREAD_STATE_SEARCHING;

    /* check pending fibers */
    {
      roa_spin_lock_aquire(&tls->fiber_lock);
      roa_spin_lock_aquire(&tls->job_lock);

			unsigned blocked_count = roa_array_size(tls->blocked_fiber_batch_ids);
			unsigned batch_count = roa_array_size(tls->batch_ids);

			unsigned i = 0;
      unsigned j;
      int unblocked_fiber_index = -1;

      /* search for some work */
      for(i = 0; i < blocked_count; ++i)
      {
        uint32_t search_for = tls->blocked_fiber_batch_ids[i];

        for (j = 0; j < batch_count; ++j)
        {
          if (tls->batch_ids[j] == search_for)
          {
            break;
          }
        }

        if (j == batch_count)
        {
          unblocked_fiber_index = i;
          break;
        }
      }

      if (unblocked_fiber_index != -1)
      {
        tls->executing_fiber = tls->blocked_fibers[i];

        roa_array_erase(tls->blocked_fibers, i);
        roa_array_erase(tls->blocked_fiber_batch_ids, i);
      }

      roa_spin_lock_release(&tls->job_lock);
      roa_spin_lock_release(&tls->fiber_lock);
	
      /* do we have a fiber? execute it */
      if (tls->executing_fiber.worker_fiber)
      {
        tls->thread_status = THREAD_STATE_WORKING;

        /* fiber switch */
        {
          struct roa_fiber *worker = tls->executing_fiber.worker_fiber;
          ROA_ASSERT(worker);

          /* switch to worker ... */

          roa_fiber_switch(tls->home_fiber, worker);

          /* ... back from fiber */
        }

        /* free fiber */
        if(tls->executing_fiber.worker_fiber)
        {
          roa_spin_lock_aquire(&tls->fiber_lock);

          roa_array_push(tls->free_fiber_pool, tls->executing_fiber.worker_fiber);
          tls->executing_fiber.worker_fiber = ROA_NULL;

          roa_spin_lock_release(&tls->fiber_lock);
        }

        continue;
      }
    } /* pending fibers */

    /* check pending jobs */
    {
      roa_spin_lock_aquire(&tls->fiber_lock);
      unsigned fiber_size = roa_array_size(tls->free_fiber_pool);

      if(fiber_size)
      {
        roa_spin_lock_aquire(&tls->job_lock);

        unsigned job_count = roa_array_size(tls->pending_jobs);

        roa_spin_lock_release(&tls->job_lock);

        /* do we have a new job? create fiber and push on pending */
        if (job_count)
        {
          /* create fiber and push onto the pending list */
          struct roa_fiber *new_fiber = roa_array_back(tls->free_fiber_pool);
          roa_array_pop(tls->free_fiber_pool);

          struct job_internal desc = tls->pending_jobs[0];
          roa_array_erase(tls->pending_jobs, 0);

          struct executing_fiber exec;
          exec.worker_fiber = new_fiber;
          exec.desc = desc;

          roa_array_push(tls->blocked_fiber_batch_ids, 0);
          roa_array_push(tls->blocked_fibers, exec);
          
          roa_spin_lock_release(&tls->fiber_lock);

          tls->thread_status = THREAD_STATE_WORKING;

          continue;
        }
      }

      roa_spin_lock_release(&tls->fiber_lock);
    } /* pending jobs */

    /* steal jobs - since nothing else todo :) */
    {
      int thread_count = ctx->thread_count;
      int i;

      for (i = 0; i < thread_count; ++i)
      {
        int search_index = ((th_index + 1) % thread_count);
        struct thread_local_storage *steal_tls = &ctx->tls[search_index];

        /* if can't aquire mutex move on */
        if (roa_spin_lock_try_aquire(&steal_tls->job_lock))
        {
          /* look for jobs that aren't thread locked */
          unsigned job_count = roa_array_size(steal_tls->pending_jobs);

          if (job_count > 1)
          {
            /* steal first job */
          }

          roa_spin_lock_release(&steal_tls->job_lock);

          /* we have a job so let it queue up */
          if (job_count)
          {
            break;
          }
        } /* aquire */
      } /* search threads */
    } /* steal jobs */
    
    tls->thread_status = THREAD_STATE_WAITING;

    if(th_index > 0)
    {
      roa_signal_wait(ctx->signal_new_work, 5);

      if (tls->thread_status == THREAD_STATE_SHUTDOWN)
      {
        return ROA_NULL;
      }
    }
    else
    {
      /* main thread check to see if we are out of work */
      int j;
      int waiting = 0;

      for (j = 0; j < ctx->thread_count; ++j)
      {
        if (ctx->tls[j].thread_status == THREAD_STATE_WAITING)
        {
          waiting += 1;
        }
      }

      if (waiting == ctx->thread_count)
      {
        int k;
        for (k = 0; k < ctx->thread_count; ++k)
        {
          ctx->tls[k].thread_status = THREAD_STATE_SHUTDOWN;
        }

        return ROA_NULL;
      }
    }

  } /* while true */

  return ROA_NULL;
}


int
job_internal_find_thread_index(struct roa_job_dispatcher_ctx *ctx)
{
  roa_thread_id this_th = roa_thread_get_current_id();

  unsigned i;
  int th_index = -1;

  for (i = 0; i < ctx->thread_count; ++i)
  {
    if (ctx->thread_ids[i] == this_th)
    {
      th_index = (int)i;
      break;
    }
  }

  return th_index;
}
