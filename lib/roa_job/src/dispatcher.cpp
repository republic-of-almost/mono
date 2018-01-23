#include <roa_job/dispatcher.h>
#include <counter.hpp>
#include <job_queue.hpp>
#include <fiber_pool.hpp>
#include <roa_lib/array.h>
#include <thread.hpp>
#include <thread.hpp>
#include <fiber.hpp>
#include <config.hpp>


/* ------------------------------------------------- [ Dispatcher Config ] -- */


#ifdef _WIN32
#define FIBER_NO_RETURN
#define FIBER_UNREACHABLE
#else
#define FIBER_NO_RETURN __attribute__ ((noreturn))
#define FIBER_UNREACHABLE __builtin_unreachable()
#endif


/* -------------------------------------------------- [ Dispatcher Types ] -- */


/*
Threads access this data directly.
*/
struct roa_thread_data
{
  /* threads switch between fibers */
  struct roa_fiber *home_fiber;     /* local fiber */
  struct roa_fiber *worker_fiber;   /* fiber to execute */

                                    /* function to exec */
  void *func;                         /* function to exec on fiber */
  void *arg;                          /* arg for function */
};


enum {
  FIBER_DISPATCHER_INITIALIZING,
  FIBER_DISPATCHER_READY,
  FIBER_DISPATCHER_RUNNING,
  FIBER_DISPATCHER_DESTROY,
};


struct roa_dispatcher_ctx
{
  /* general */
  int dispatch_state;

  /* threads */
  int thread_count;
  /* array */ struct roa_thread_data  *thread_local_data;
  /* array */ struct roa_thread       *raw_threads;
  /* jobs */
  struct roa_job_queue_ctx job_queue;

  /* fibers */
  struct roa_fiber_pool_ctx fiber_pool;

  /* config */
  struct roa_dispatcher_desc desc;
};


/* ---------------------------------------------- [ Internal Dispatchers ] -- */
/*
There are two dispatchers.
One that runs on threads when no fiber is executing to find new work todo.
And another that run on the worker firbers to execute the job and yeild back.
*/


/*
Runs a fiber, the fiber should be setup with a job before this has been
switched to.
*/

void roa_internal_fiber_executer(void *arg) FIBER_NO_RETURN;

void
roa_internal_fiber_executer(void *arg)
{
  /* param check */
  FIBER_ASSERT(arg);

  /* get ctx */
  struct roa_dispatcher_ctx *ctx = (struct roa_dispatcher_ctx*)arg;
  FIBER_ASSERT(ctx);

  for (;;)
  {
    /* fiber work - we don't know what thread we are on */
    {
      struct roa_thread *thds = ctx->raw_threads;
      const int thds_count = ctx->thread_count;
      const int thd_index = roa_thread_find_this(thds, thds_count);

      struct roa_thread_data *tls = &ctx->thread_local_data[thd_index];

      FIBER_ASSERT(tls->worker_fiber);
      FIBER_ASSERT(tls->home_fiber);

      /* exec job */
      roa_job_func job_func = (roa_job_func)tls->func;
      void *job_arg = tls->arg;

      FIBER_ASSERT(job_func);

      job_func(ctx, job_arg);
    }

    /* switch back - we might be on a different thread */
    {
      struct roa_thread *thds = ctx->raw_threads;
      const int thds_count = ctx->thread_count;
      const int thd_index = roa_thread_find_this(thds, thds_count);

      struct roa_thread_data *tls = &ctx->thread_local_data[thd_index];

      FIBER_ASSERT(tls->worker_fiber);
      FIBER_ASSERT(tls->home_fiber);

      /* return to the threads fiber dispatcher */
      roa_fiber_switch(tls->worker_fiber, tls->home_fiber);
    }
  }

  /* this function doesn't return ever */
  FIBER_UNREACHABLE;
}


/* Dispatcher finds jobs and fibers and kicks them off */
#ifdef _WIN32
unsigned __stdcall
roa_internal_fiber_dispatcher(void *arg)
#else
void*
roa_internal_fiber_dispatcher(void *arg)
#endif
{
  /* param check */
  FIBER_ASSERT(arg);

  /* setup thread */
  struct roa_dispatcher_ctx *ctx = (struct roa_dispatcher_ctx*)arg;
  FIBER_ASSERT(ctx);

  /* threads can startup before setup has finished */
  while (ctx->dispatch_state <= FIBER_DISPATCHER_INITIALIZING) {}

  /* create a fiber for this thread, so we can jump in and out */
  struct roa_thread *thds = ctx->raw_threads;
  const int thds_count = ctx->thread_count;
  const int thd_index = roa_thread_find_this(thds, thds_count);

  struct roa_thread_data *tls = &ctx->thread_local_data[thd_index];
  FIBER_ASSERT(tls);

  tls->home_fiber = 0;
  tls->worker_fiber = 0;

  roa_fiber_create(&tls->home_fiber, 0, (void*)0);
  FIBER_ASSERT(tls->home_fiber);

  /* spin while we are waiting for startup */
  while (ctx->dispatch_state <= FIBER_DISPATCHER_READY) {}

  for (;;)
  {
    /* must start in a good state */
    FIBER_ASSERT(tls->worker_fiber == 0);
    FIBER_ASSERT(tls->home_fiber != 0);

    /* check we still have work in the system */
    {
      const int has_pending_jobs = roa_job_queue_has_work(&ctx->job_queue);
      const int has_pending_fibers = roa_fiber_pool_has_work(&ctx->fiber_pool);

      /* all done */
      if ((has_pending_fibers + has_pending_jobs) == 0)
      {
        break;
      }
    }

    /* switch to a fiber - pending or a new job */
    {
      tls->worker_fiber = roa_fiber_pool_next_pending(&ctx->fiber_pool);

      /* pending fiber */
      if (tls->worker_fiber)
      {
        /* fiber already has function/arg and has alreaded executed call */
        FIBER_ASSERT(tls->worker_fiber);
        FIBER_ASSERT(roa_fiber_get_user_id(tls->worker_fiber));

        /* switch to fiber */
        roa_fiber_switch(tls->home_fiber, tls->worker_fiber);
      }

      /* find new job and fiber */
      else
      {
        /* setup for new fiber */
        const unsigned job_id = roa_job_queue_next(
          &ctx->job_queue,
          thd_index,
          &tls->func,
          &tls->arg
        );

        /* didn't get a job - bail come back later */
        if (!job_id)
        {
          continue;
        }

        /* this should spin until one is free */
        tls->worker_fiber = roa_fiber_pool_next_free(&ctx->fiber_pool);

        /* are we in the right state to switch */
        FIBER_ASSERT(tls->worker_fiber);
        FIBER_ASSERT(tls->func);
        FIBER_ASSERT(job_id);

        roa_fiber_set_user_id(tls->worker_fiber, job_id);

        /* switch to fiber */
        roa_fiber_switch(tls->home_fiber, tls->worker_fiber);
      }
    }

    /* -- back from fiber -- */

    /* if fiber finished work */
    if (tls->worker_fiber)
    {
      const int job_id = roa_fiber_get_user_id(tls->worker_fiber);
      FIBER_ASSERT(job_id);

      roa_job_queue_clear(&ctx->job_queue, job_id);

      /* return fiber to pool */
      roa_fiber_pool_done(&ctx->fiber_pool, tls->worker_fiber);

      /* reset tls */
      tls->worker_fiber = 0;
    }
  } /* while has work */

    /* time to quit */

  return 0;
}


/* ----------------------------------------------- [ Dispatcher Lifetime ] -- */
/*
Functions that deal with the lifetime of the fiber context.
*/

void
roa_dispatcher_create(
  struct roa_dispatcher_ctx **c,
  const struct roa_dispatcher_desc *override_desc)
{
  /* param assert */
  FIBER_ASSERT(c);

  /* init ctx */
  struct roa_dispatcher_ctx *new_ctx = NULL;
  {
    int bytes = sizeof(struct roa_dispatcher_ctx);
    new_ctx = (struct roa_dispatcher_ctx*)FIBER_MALLOC(bytes);
    FIBER_MEMZERO(new_ctx, bytes);
    new_ctx->dispatch_state = FIBER_DISPATCHER_INITIALIZING;
  }

  /* config */
  {
    /* default config */
    new_ctx->desc.free_cores = 1;
    new_ctx->desc.max_fibers = 128;
    new_ctx->desc.max_jobs = 1024;

    if (override_desc)
    {
      struct roa_dispatcher_desc *this_desc = &new_ctx->desc;

      this_desc->free_cores = override_desc->free_cores;

      if (override_desc->max_fibers > 0)
      {
        this_desc->max_fibers = override_desc->max_fibers;
      }

      if (override_desc->max_jobs > 0)
      {
        this_desc->max_jobs = override_desc->max_jobs;
      }
    }
  }

  /* create fiber and job pools */
  {
    roa_job_queue_create(&new_ctx->job_queue, new_ctx->desc.max_jobs);

    roa_fiber_pool_create(
      &new_ctx->fiber_pool,
      (void*)roa_internal_fiber_executer,
      (void*)new_ctx,
      new_ctx->desc.max_fibers
    );
  }

  /* init thread data */
  {
    int core_count = roa_thread_core_count();
    new_ctx->thread_count = 1;

    if (new_ctx->desc.free_cores >= 0)
    {
      new_ctx->thread_count = core_count - new_ctx->desc.free_cores;
    }

    roa_array_create(new_ctx->raw_threads, new_ctx->thread_count);
    roa_array_resize(new_ctx->raw_threads, new_ctx->thread_count);

    roa_array_create(new_ctx->thread_local_data, new_ctx->thread_count);
    roa_array_resize(new_ctx->thread_local_data, new_ctx->thread_count);

    /* setup main thread */
    struct roa_thread this_thread = roa_thread_create_this();
    {
      new_ctx->raw_threads[0] = this_thread;
      new_ctx->thread_local_data[0].worker_fiber = 0;
      new_ctx->raw_threads[0] = this_thread;
    }

    /* threads for the other cores */
    for (int i = 1; i < new_ctx->thread_count; ++i)
    {
      struct roa_thread_desc desc;
      desc.func = roa_internal_fiber_dispatcher;
      desc.arg = (void*)new_ctx;
      desc.affinity = i;

      //new_ctx->raw_threads[i] = roa_thread_create(&desc);
      roa_thread_create(&new_ctx->raw_threads[i], &desc);
    }

    /* check */
    printf("create: ");
    roa_thread_find_this(new_ctx->raw_threads, new_ctx->thread_count);

    /* th logging */
    {
      FIBER_LOG_NUMBER("Core Count: ", core_count);
      FIBER_LOG_NUMBER("Thread Count: ", new_ctx->thread_count);
    }
  }

  /* allow threads to setup */
  new_ctx->dispatch_state = FIBER_DISPATCHER_READY;

  /* all good */
  *c = new_ctx;
}


void
roa_dispatcher_destroy(struct roa_dispatcher_ctx **c)
{
  /* param assert */
  FIBER_ASSERT(c);

  (*c)->dispatch_state = FIBER_DISPATCHER_DESTROY;

  /* destroy threads */
  {
    int th_count = (*c)->thread_count;
    
    for (int i = 1; i < th_count; ++i)
    {
      roa_thread_destroy(&(*c)->raw_threads[i]);
    }

    roa_array_destroy((*c)->thread_local_data);
    roa_array_destroy((*c)->raw_threads);
  }

  roa_job_queue_destroy(&(*c)->job_queue);
  roa_fiber_pool_destroy(&(*c)->fiber_pool);

  FIBER_FREE(*c);
}


/* ------------------------------------------------ [ Dispatcher Actions ] -- */


void
roa_dispatcher_run(
  struct roa_dispatcher_ctx *c)
{
  /* param assert */
  {
    FIBER_ASSERT(c);
  }

  /* allow threads to start executing, and have this thread join in the fun */
  {
    c->dispatch_state = FIBER_DISPATCHER_RUNNING;

    roa_internal_fiber_dispatcher(c);
  }

  /* wait for threads to clean up */
  {
    /* exclude first thread as its main thread */
    //roa_thread **ths = &c->raw_threads[1];
    const unsigned th_count = c->thread_count - 1;

    //roa_thread_join(ths, th_count);
  }

  printf("run: ");
  roa_thread_find_this(c->raw_threads, c->thread_count);


  FIBER_LOG("Dispatch shutdown");
}


unsigned
roa_dispatcher_add_jobs(
  struct roa_dispatcher_ctx *c,
  struct roa_job_desc *desc,
  int job_count)
{
  /* param assert */
  {
    FIBER_ASSERT(c);
    FIBER_ASSERT(desc);
    FIBER_ASSERT(job_count);
  }

  const int th_id = roa_thread_find_this(c->raw_threads, c->thread_count);

  return roa_job_queue_add_batch(&c->job_queue, desc, job_count, th_id);
}


void
roa_dispatcher_wait_for_counter(
  struct roa_dispatcher_ctx *ctx,
  unsigned marker)
{
  /* param assert */
  {
    FIBER_ASSERT(ctx);
    FIBER_ASSERT(marker);
  }

  /* get tls */
  struct roa_thread_data *tls = NULL;
  {
    struct roa_thread *thds = ctx->raw_threads;
    const int thds_count = ctx->thread_count;
    const int thd_index = roa_thread_find_this(thds, thds_count);

    tls = &ctx->thread_local_data[thd_index];
  }

  /* check state */
  {
    FIBER_ASSERT(tls);

    /* if this happens you might have called this from the main thread */
    /* you need to have the main thread join in the fun with */
    /* roa_dispatcher_run(...) */
    FIBER_ASSERT(tls->worker_fiber);
  }

  /* block this fiber */
  struct roa_counter *counter = NULL;
  {
    counter = roa_job_queue_batch_block(&ctx->job_queue, marker);

    if (counter == NULL)
    {
      /* batch already finished - continue */
      return;
    }

    counter->has_pending = 1;
    roa_fiber_pool_block(&ctx->fiber_pool, tls->worker_fiber, counter);
  }

  /* switch back to home fiber */
  {
    struct roa_fiber *worker = tls->worker_fiber;
    tls->worker_fiber = 0;
    roa_fiber_switch(worker, tls->home_fiber);
  }

  /* fiber back on thread */
  {
    FIBER_ASSERT(counter);
    FIBER_ASSERT(counter->has_pending == 1);

    roa_job_queue_batch_unblock(&ctx->job_queue, counter);
  }
}


/* ------------------------------------------------- [ Dispatcher Config ] -- */


#undef FIBER_NO_RETURN
#undef FIBER_UNREACHABLE
