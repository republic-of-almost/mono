#include <roa_job/roa_job.h>
#include <thread_dispatch/thread_process.h>
#include <thread_dispatch/thread_local_storage.h>
#include <roa_lib/thread.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/array.h>
#include <ctx/context.h>
#include <fiber/fiber.h>
#include <jobs/jobs.h>
#include <roa_lib/spin_lock.h>


void
fiber_process(void *arg)
{
  ROA_UNUSED(arg);
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

    /* setup tls */
    if (tls)
    {
      roa_spin_lock_init(&tls->job_lock);
      roa_spin_lock_init(&tls->fiber_lock);

      roa_array_create_with_capacity(tls->batches, 32);
      roa_array_create_with_capacity(tls->pending_jobs, 128);

      roa_array_create_with_capacity(tls->blocked_fibers, 128);
      roa_array_create_with_capacity(tls->free_fiber_pool, 128);
    }
  }
  
  /*
    first check pending fibers, if found switch to.
    then check pending jobs, if found create pending fiber.
    then try steal work from other threads, and create a pending job.
  */
  while (1)
  {
    /* check pending fibers */
    {
      roa_spin_lock_aquire(&tls->fiber_lock);

      roa_spin_lock_release(&tls->fiber_lock);

      /* do we have a fiber? execute it */
      if (0)
      {
        /* start again */
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
          struct fiber *new_fiber = roa_array_back(tls->free_fiber_pool);
          roa_array_pop(tls->free_fiber_pool);

          roa_spin_lock_release(&tls->fiber_lock);
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
  } /* while true */

  return ROA_NULL;
}