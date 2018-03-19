#include <roa_job/roa_job.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/sys.h>
#include <roa_lib/log.h>
#include <roa_lib/thread.h>
#include <roa_lib/array.h>
#include <ctx/context.h>
#include <thread_dispatch/thread_local_storage.h>
#include <thread_dispatch/thread_process.h>


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
      ROA_LOG_INFO("Job Dispatcher has more threads than cores");
      thread_count = 2;
    }

    new_ctx->thread_count = thread_count;

    /* create threads */
    {
      roa_array_create_with_capacity(new_ctx->tls, thread_count);
      roa_array_create_with_capacity(new_ctx->threads, thread_count);
      roa_array_create_with_capacity(new_ctx->thread_ids, thread_count);

      int i;

      for (i = 0; i < thread_count; ++i)
      {
        /* storeage for new thread */
        struct thread_local_storage new_tls;
        ROA_MEM_ZERO(new_tls);
        roa_array_push(new_ctx->tls, new_tls);

        /* thread proc will fill this in */
        roa_thread_id id;
        ROA_MEM_ZERO(id);
        roa_array_push(new_ctx->thread_ids, id);

        /* create new thread */
        roa_thread th;
        ROA_MEM_ZERO(th);

        /* thread zero is main thread */
        if (i != 0)
        {
          struct thread_arg *arg = roa_zalloc(sizeof(*arg));
          arg->ctx = new_ctx;
          arg->roa_thread_id = &new_ctx->thread_ids[i];
          th = roa_thread_create(thread_process, arg, 1024, i);
        }
        roa_array_push(new_ctx->threads, th);
      }
    }

    *ctx = new_ctx;
  }
}

#include <windows.h>
void
roa_job_dispatcher_ctx_run(
  roa_job_dispatcher_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  /* signal start */

  ctx->threads[0] = roa_thread_create_self();

  struct thread_arg *arg = roa_zalloc(sizeof(*arg));
  arg->ctx = ctx;
  arg->roa_thread_id = &ctx->thread_ids[0];

  /* become a dispatcher also */
  thread_process(arg);

  /* quitting ... */
}


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
    int i;
    for (i = 0; i < kill_ctx->thread_count; ++i)
    {
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

}