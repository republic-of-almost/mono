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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define THREAD_PROCESS_DEBUG_OUTPUT 1


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
      char buffer[1024];
      ROA_MEM_ZERO(buffer);

      struct job_internal job_data = tls->executing_fiber.desc;
      struct roa_job_desc desc = job_data.desc;

      roa_job_fn job_func = (roa_job_fn)desc.func;
      void *job_arg = desc.arg;

      sprintf(buffer, "th: %d - func: %p arg %p", th_index, job_func, job_arg);
      printf("%s\n", buffer);

      job_func(ctx, job_arg);
    }

    /* decrement job counter */
    /* clear executing as its done */
    {
      /* decrement job count */
      {
        roa_atomic_int_dec(tls->executing_fiber.desc.counter);

        /*
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
            int count = roa_atomic_int_dec(&batch->counter);

            if (count <= 0)
            {
              roa_array_erase(tls->batch_ids, i);
              roa_array_erase(tls->batches, i);
            }
            
            break;
          }
        }
        */

        /*roa_spin_lock_release(&tls->job_lock);*/
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


ROA_BOOL
thread_internal_run_fiber(
  struct thread_local_storage *tls)
{
  ROA_BOOL return_val = ROA_FALSE;

	/* lock jobs to copy batch ids */
	uint32_t *batch_id = ROA_NULL;
	unsigned batch_count = 0;
	{
		roa_spin_lock_aquire(&tls->job_lock);
		
		batch_count = roa_array_size(tls->batch_ids);

		unsigned bytes = sizeof(tls->batch_ids[0]) * batch_count;

		batch_id = roa_alloc(bytes);

		memcpy(batch_id, tls->batch_ids, bytes);

		roa_spin_lock_release(&tls->job_lock);
	}

	/* lock fiber to check batch ids against */
	{
		unsigned blocked_count = roa_array_size(tls->blocked_fiber_batch_ids);

		unsigned i;
		unsigned j;
		int unblocked_fiber_index = -1;

	  /* search for some work */
		for (i = 0; i < blocked_count; ++i)
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

	  /* do we have a fiber? execute it */
		if (tls->executing_fiber.worker_fiber)
		{
			return_val = ROA_TRUE;

	    /* fiber switch */
		  {
			  struct roa_fiber *worker = tls->executing_fiber.worker_fiber;
				ROA_ASSERT(worker);

	      /* switch to worker ... */

		    roa_fiber_switch(tls->home_fiber, worker);

			  /* ... back from fiber */
	    }

		  /* free fiber */
			if (tls->executing_fiber.worker_fiber)
	    {
		    roa_spin_lock_aquire(&tls->fiber_lock);
	
		    roa_array_push(tls->free_fiber_pool, tls->executing_fiber.worker_fiber);
			  tls->executing_fiber.worker_fiber = ROA_NULL;
	    }
		}

		roa_spin_lock_release(&tls->fiber_lock);
	}

	/* cleanup */
	{
		roa_free(batch_id);
	}
  
  return return_val;
}


ROA_BOOL
thread_internal_remove_cleared_batches(
  struct thread_local_storage *tls)
{
  ROA_BOOL return_val = ROA_FALSE;

  roa_spin_lock_aquire(&tls->job_lock);
  unsigned batch_count = roa_array_size(tls->batch_ids);

  int i;
  for (i = 0; i < batch_count; ++i)
  {
    struct job_batch *batch = &tls->batches[i];
    int count = roa_atomic_int_load(batch->counter);

    if (count <= 0)
    {
      roa_free(batch->counter);

      roa_array_erase(tls->batch_ids, i);
      roa_array_erase(tls->batches, i);

      return_val = ROA_TRUE;
    }
  }

  roa_spin_lock_release(&tls->job_lock);

  return return_val;
}


ROA_BOOL
thread_internal_check_pending(
  struct thread_local_storage *tls)
{
  ROA_BOOL return_val = ROA_FALSE;

  roa_spin_lock_aquire(&tls->fiber_lock);
  unsigned fiber_size = roa_array_size(tls->free_fiber_pool);

  if (fiber_size)
  {
    roa_spin_lock_aquire(&tls->job_lock);

    unsigned job_count = roa_array_size(tls->pending_jobs);

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
      roa_spin_lock_release(&tls->job_lock);

      return_val = ROA_TRUE;
    }

    roa_spin_lock_release(&tls->job_lock);
  }

  roa_spin_lock_release(&tls->fiber_lock);

  return return_val;
}


ROA_BOOL
thread_internal_steal_jobs(
  struct thread_local_storage *tls,
  struct roa_job_dispatcher_ctx *ctx,
  unsigned th_index)
{
  ROA_BOOL return_val = ROA_FALSE;

	struct job_internal steal_job;
	ROA_BOOL stole_a_job = ROA_FALSE;

	/* search for work to steal */
	{
		if(ROA_IS_ENABLED(THREAD_PROCESS_DEBUG_OUTPUT))
		{
			/*printf("Thread %d looking to steal \n", th_index);*/
		}

		int th_count = ctx->thread_count;
		int i;

		for(i = 0; i < th_count; ++i)
		{
			int steal_index = ((th_index + 1 + i) % th_count);
			struct thread_local_storage *steal_tls = &ctx->tls[steal_index];

			/* shouldn't happen but just incase */
			if(steal_tls == tls)
			{
				continue;
			}

			/* try and get lock */
			if(roa_spin_lock_try_aquire(&steal_tls->job_lock))
			{
				struct job_internal *steal_jobs = steal_tls->pending_jobs;
				unsigned job_count = roa_array_size(steal_jobs);

				int j;
				for(j = 0; j < job_count; ++j)
				{
					struct job_internal steal = steal_jobs[j];

					if(steal.desc.thread_locked == ROA_FALSE)
					{
						steal_job = steal;
						stole_a_job = ROA_TRUE;
						roa_array_erase(steal_jobs, j);

						if(ROA_IS_ENABLED(THREAD_PROCESS_DEBUG_OUTPUT))
						{
							printf("Thread %d stole from thread %d \n", th_index, steal_index);
						}

						break;
					}
				}

				roa_spin_lock_release(&steal_tls->job_lock);
			}
		}
	}

	/* lock this tls and apply work */
	if(stole_a_job)
	{
		steal_job.desc.thread_locked = ROA_TRUE;

		roa_spin_lock_aquire(&tls->job_lock);

		roa_array_push(tls->pending_jobs, steal_job);

		roa_spin_lock_release(&tls->job_lock);

		return_val = ROA_TRUE;
	}

  return return_val;
}


ROA_BOOL
thread_internal_wait_or_quit(
  struct thread_local_storage *tls,
  struct roa_job_dispatcher_ctx *ctx,
  unsigned th_index
)
{
  ROA_BOOL stay_alive = ROA_TRUE;

  if (th_index == 0)
  {
    /* main thread check to see if we are out of work */
    int i;
		int th_count = ctx->thread_count;
    ROA_BOOL work = ROA_FALSE;

		/* lock all thread jobs */
		for(i = 0; i < th_count; ++i)
		{
			roa_spin_lock_aquire(&ctx->tls[i].job_lock);
		}

		/* check batches */
		for(i = 0; i < th_count; ++i)
		{
			unsigned count = roa_array_size(ctx->tls[i].batches);

			if(count)
			{
				work = ROA_TRUE;
				break;
			}
		}

		/* unlock all */
		for(i = 0; i < th_count; ++i)
		{
			roa_spin_lock_release(&ctx->tls[i].job_lock);
		}

		/* signal quit if required */
		if(work == ROA_FALSE)
		{
			for(i = 0; i < th_count; ++i)
			{
				ctx->tls[i].thread_status = TLS_QUIT; 
			}

			stay_alive = ROA_FALSE;
		}
  }
	else if(tls->thread_status == TLS_QUIT)
	{
		stay_alive = ROA_FALSE;
	}

  return stay_alive;
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
  unsigned th_count = ctx->thread_count;
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

  tls->thread_status = TLS_RUNNING;


  /* if this loop gets to the bottom it exits */
  while (1)
  {
    /* remove completed batches */
    if (thread_internal_remove_cleared_batches(tls))
    {
      continue;
    }

    /* check pending fibers */
    if (thread_internal_run_fiber(tls))
    {
      continue;
    }

    /* check pending jobs */
    if (thread_internal_check_pending(tls))
    {
      continue;
    }
    
    /* steal jobs - since nothing else todo :) */
    if (thread_internal_steal_jobs(tls, ctx, th_index))
    {
      continue;
    }
    
    if (thread_internal_wait_or_quit(tls, ctx, th_index))
    {
      continue;
    }

		if(ROA_IS_ENABLED(THREAD_PROCESS_DEBUG_OUTPUT))
		{
			printf("Exiting th %d\n", th_index);
		}

    return ROA_NULL;
    
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
