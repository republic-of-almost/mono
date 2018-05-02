#include <roa_job/roa_job.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/sys.h>
#include <roa_lib/log.h>
#include <roa_lib/thread.h>
#include <roa_lib/atomic.h>
#include <roa_lib/spin_lock.h>
#include <roa_lib/array.h>
#include <jobs/jobs.h>
#include <fiber/fiber.h>
#include <ctx/context.h>
#include <thread_dispatch/thread_local_storage.h>
#include <thread_dispatch/thread_process.h>


/* ------------------------------------------------------------ [ config ] -- */


#ifndef ROA_JOB_CPU_AFFINITY
#define ROA_JOB_CPU_AFFINITY 0
#endif


#ifndef ROA_JOB_DEBUG_OUTPUT
#define ROA_JOB_DEBUG_OUTPUT 0
#endif


#ifdef ROA_JOB_DEBUG_OUTPUT
#include <stdio.h>
#endif


/* ---------------------------------------------------------- [ lifetime ] -- */


void
roa_job_dispatcher_ctx_create(
  roa_job_dispatcher_ctx_t *ctx,
  struct roa_job_dispatcher_setup_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);

  /* create new ctx */
  struct roa_job_dispatcher_ctx *new_ctx = roa_zalloc(sizeof(*new_ctx));

  if(new_ctx)
  {
    int max_cores = 32;
    int free_cores = 1;

    if (desc != ROA_NULL)
    {
      max_cores = desc->max_cores;
      free_cores = desc->spare_cores;
    }

    /* get core count and create accordingly */
    int core_count = roa_cpu_count();

    int thread_count = max_cores < core_count ? max_cores : core_count;
    thread_count -= free_cores;

    if (thread_count < 0)
    {
      thread_count = 2;
    }
    thread_count = 3;
    new_ctx->thread_count = thread_count;

		ROA_LOG_INFO(
			"Job Dispatcher: Core Count %d, Thread Count %d",
			core_count,
			thread_count);

    /* create signals */
    new_ctx->signal_start = roa_signal_create();
    new_ctx->signal_new_work = roa_signal_create();

    /* create threads */
    {
      roa_array_create_with_capacity(new_ctx->tls, thread_count);
      roa_array_create_with_capacity(new_ctx->threads, thread_count);
      roa_array_create_with_capacity(new_ctx->thread_ids, thread_count);

      int i;

      for (i = 0; i < thread_count; ++i)
      {
        /* new tls and tls setup */
				{
	        struct thread_local_storage new_tls;
		      ROA_MEM_ZERO(new_tls);

					roa_spin_lock_init(&new_tls.job_lock);
		      roa_spin_lock_init(&new_tls.fiber_lock);

				  roa_array_create_with_capacity(new_tls.batches, 64);
					roa_array_create_with_capacity(new_tls.batch_ids, 64);
		      roa_array_create_with_capacity(new_tls.pending_jobs, 256);

          unsigned fiber_count = 32;

		      roa_array_create_with_capacity(new_tls.blocked_fibers, fiber_count);
					roa_array_create_with_capacity(new_tls.blocked_fiber_batch_ids, fiber_count);
		      roa_array_create_with_capacity(new_tls.free_fiber_pool, fiber_count);

          unsigned j;

          for (j = 0; j < fiber_count; ++j)
          {
            struct roa_fiber *fi;
            roa_fiber_create(&fi, fiber_process, (void*)new_ctx);

            roa_array_push(new_tls.free_fiber_pool, fi);
          }

					roa_array_push(new_ctx->tls, new_tls);
				}

        /* thread proc will fill this in */
				{
			    roa_thread_id id;
		      ROA_MEM_ZERO(id);
	        roa_array_push(new_ctx->thread_ids, id);
				}

        /* create new thread */
				{
		      roa_thread th;
	        ROA_MEM_ZERO(th);

					/* thread zero is main thread */
				  if (i != 0)
			    {
		        struct thread_arg *arg = roa_zalloc(sizeof(*arg));
	          arg->ctx = new_ctx;
						arg->roa_thread_id = &new_ctx->thread_ids[i];

					  th = roa_thread_create(thread_process, arg, 1024, i);

            if (ROA_IS_ENABLED(ROA_JOB_CPU_AFFINITY))
            {
              roa_thread_set_affinity(th, i);
            }
				  }
					else
					{
						/* get now so that we can add jobs */
						new_ctx->thread_ids[0] = roa_thread_get_current_id();

					}
					roa_array_push(new_ctx->threads, th);
				}
      }
    }

    *ctx = new_ctx;
  }
}


void
roa_job_dispatcher_ctx_run(
  roa_job_dispatcher_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  /* signal start */
  ctx->threads[0] = roa_thread_create_self();

  if (ROA_IS_ENABLED(ROA_JOB_CPU_AFFINITY))
  {
    roa_thread_set_affinity(ctx->threads[0], 0);
  }

  struct thread_arg *arg = roa_zalloc(sizeof(*arg));
  arg->ctx = ctx;
  arg->roa_thread_id = &ctx->thread_ids[0];

  /* become a dispatcher also */
  thread_process(arg);

  /* quitting ... */
}

#include <stdio.h>
void
roa_job_dispatcher_ctx_destroy(
  roa_job_dispatcher_ctx_t *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(*ctx);

  struct roa_job_dispatcher_ctx *kill_ctx = *ctx;

  if(kill_ctx)
  {
    /* free threads and data */
    unsigned i;
    for (i = 1; i < kill_ctx->thread_count; ++i)
    {
      if(ROA_IS_ENABLED(ROA_JOB_DEBUG_OUTPUT))
      {
        printf("th destroy %d\n", i);
      }
      roa_thread_destroy(kill_ctx->threads[i]);
    }

    /* destroy containers */
    roa_array_destroy(kill_ctx->threads);
    roa_array_destroy(kill_ctx->thread_ids);
    roa_array_destroy(kill_ctx->tls);

    roa_free(kill_ctx);
  }
}


void
roa_job_dispatcher_ctx_get_desc(
  roa_job_dispatcher_ctx_t ctx,
  struct roa_job_dispatcher_desc *desc)
{
  ROA_UNUSED(ctx);
  ROA_UNUSED(desc);
}
