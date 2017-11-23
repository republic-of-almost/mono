#include <optio/dispatcher.h>
#include <job_queue.h>
#include <fiber_pool.h>
#include <roa_array.h>
#include <thread.h>
#include <fiber.h>
#include <config.h>


/* ------------------------------------------------- [ Dispatcher Config ] -- */


#define FIBER_NO_RETURN __attribute__ ((noreturn))
#define FIBER_UNREACHABLE __builtin_unreachable()


/* -------------------------------------------------- [ Dispatcher Types ] -- */


/*
  Threads access this data directly.
*/
struct optio_thread_data
{
  struct optio_fiber *home_fiber;     /* local fiber */
  struct optio_fiber *worker_fiber;   /* fiber to execute */
  
  void *func;                       /* function to exec on fiber */
  void *arg;                        /* arg for function */
};


enum {
  FIBER_DISPATCHER_INITIALIZING,
  FIBER_DISPATCHER_READY,
  FIBER_DISPATCHER_RUNNING,
  FIBER_DISPATCHER_DESTROY,
};



struct optio_dispatcher_ctx
{
  /* general */
  int dispatch_state;

  /* threads */
  int thread_count;
  /* array */ struct optio_thread_data  *thread_local_data;
  /* array */ struct optio_thread       **raw_threads;

  /* jobs */
  struct optio_job_queue_ctx job_queue;
  
  /* fibers */
  struct optio_fiber_pool_ctx fiber_pool;

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
void optio_internal_fiber_executer(void *arg) FIBER_NO_RETURN;

void
optio_internal_fiber_executer(void *arg)
{
  /* param check */
  FIBER_ASSERT(arg);

  /* get ctx */
  struct optio_dispatcher_ctx *ctx = (struct optio_dispatcher_ctx*)arg;
  FIBER_ASSERT(ctx);
 
  for(;;)
  {
    /* fiber work - we don't know what thread we are on */
    {
      struct optio_thread **thds = ctx->raw_threads;
      const int thds_count = ctx->thread_count;
      const int thd_index = optio_thread_find_this(thds, thds_count);
      
      struct optio_thread_data *tls = &ctx->thread_local_data[thd_index];
      
      FIBER_ASSERT(tls->worker_fiber);
      FIBER_ASSERT(tls->home_fiber);
      
      /* exec job */
      optio_job_func func = tls->func;
      void *arg = tls->arg;
      
      FIBER_ASSERT(func);
      
      func(ctx, arg);
    }
    
    /* switch back - we might be on a different thread */
    {
      struct optio_thread **thds = ctx->raw_threads;
      const int thds_count = ctx->thread_count;
      const int thd_index = optio_thread_find_this(thds, thds_count);
      
      struct optio_thread_data *tls = &ctx->thread_local_data[thd_index];
      
      FIBER_ASSERT(tls->worker_fiber);
      FIBER_ASSERT(tls->home_fiber);

      /* return to the threads fiber dispatcher */
      optio_fiber_switch(tls->worker_fiber, tls->home_fiber);
    }
  }
  
  /* this function doesn't return ever */
  FIBER_UNREACHABLE;
}


/* Dispatcher finds jobs and fibers and kicks them off */
void*
optio_internal_fiber_dispatcher(void *arg)
{
  /* param check */
  FIBER_ASSERT(arg);

  /* setup thread */
  struct optio_dispatcher_ctx *ctx = (struct optio_dispatcher_ctx*)arg;
  FIBER_ASSERT(ctx);
  
  /* threads can startup before setup has finished */
  while(ctx->dispatch_state <= FIBER_DISPATCHER_INITIALIZING) {}

  /* create a fiber for this thread, so we can jump in and out */
  struct optio_thread **thds = ctx->raw_threads;
  const int thds_count = ctx->thread_count;
  const int thd_index = optio_thread_find_this(thds, thds_count);

  struct optio_thread_data *tls = &ctx->thread_local_data[thd_index];
  FIBER_ASSERT(tls);
  
  tls->home_fiber = 0;
  tls->worker_fiber = 0;
  
  optio_fiber_create(&tls->home_fiber, 0, (void*)0);
  FIBER_ASSERT(tls->home_fiber);

  /* spin while we are waiting for startup */
  while(ctx->dispatch_state <= FIBER_DISPATCHER_READY) {}
  
  for(;;)
  {
    /* must start in a good state */
    FIBER_ASSERT(tls->worker_fiber == 0);
    FIBER_ASSERT(tls->home_fiber != 0);

    /* check we still have work in the system */
    {
      const int has_pending_jobs = optio_job_queue_has_work(&ctx->job_queue);
      const int has_pending_fibers = optio_fiber_pool_has_work(&ctx->fiber_pool);
      
      /* all done */
      if((has_pending_fibers + has_pending_jobs) == 0)
      {
        break;
      }
    }
    
    /* switch to a fiber - pending or a new job */
    {
      tls->worker_fiber = optio_fiber_pool_next_pending(&ctx->fiber_pool);
      
      /* pending fiber */
      if(tls->worker_fiber)
      {
        /* fiber already has function/arg and has alreaded executed call */
        FIBER_ASSERT(tls->worker_fiber);
        FIBER_ASSERT(optio_fiber_get_user_id(tls->worker_fiber));
      
        /* switch to fiber */
        optio_fiber_switch(tls->home_fiber, tls->worker_fiber);
      }
      
      /* find new job and fiber */
      else
      {
        /* setup for new fiber */
        tls->func = 0;
        tls->arg = 0;
        
        const int job_id = optio_job_queue_next(
          &ctx->job_queue,
          &tls->func,
          &tls->arg
        );
        
        /* didn't get a job - bail come back later */
        if(!job_id)
        {
          continue;
        }
        
        /* this should spin until one is free */
        tls->worker_fiber = optio_fiber_pool_next_free(&ctx->fiber_pool);
        
        /* are we in the right state to switch */
        FIBER_ASSERT(tls->worker_fiber);
        FIBER_ASSERT(tls->func);
        FIBER_ASSERT(job_id);
        
        optio_fiber_set_user_id(tls->worker_fiber, job_id);
        
        /* switch to fiber */
        optio_fiber_switch(tls->home_fiber, tls->worker_fiber);
      }
    }
    
    /* -- back from fiber -- */
    
    /* if fiber finished work */
    if(tls->worker_fiber)
    {
      const int job_id = optio_fiber_get_user_id(tls->worker_fiber);
      FIBER_ASSERT(job_id);
      
      const int batch_cleared = optio_job_queue_clear(&ctx->job_queue, job_id);
      
      /* batch cleared - unblock any pending */
      if(batch_cleared)
      {
        optio_fiber_pool_unblock(&ctx->fiber_pool, batch_cleared);
      }
      
      /* return fiber to pool */
      optio_fiber_pool_done(&ctx->fiber_pool, tls->worker_fiber);
      
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
optio_dispatcher_create(struct optio_dispatcher_ctx **c)
{
  /* param assert */
  FIBER_ASSERT(c);

  /* init ctx */
  struct optio_dispatcher_ctx *new_ctx = NULL;
  {
    int bytes = sizeof(struct optio_dispatcher_ctx);
    new_ctx = (struct optio_dispatcher_ctx*)FIBER_MALLOC(bytes);
    FIBER_MEMZERO(new_ctx, bytes);
    new_ctx->dispatch_state = FIBER_DISPATCHER_INITIALIZING;
  }
  
  /* create fiber and job pools */
  {
    optio_job_queue_create(&new_ctx->job_queue);
    
    optio_fiber_pool_create(
      &new_ctx->fiber_pool,
      optio_internal_fiber_executer,
      (void*)new_ctx
    );
  }

  /* init thread data */
  {
    int core_count = optio_thread_core_count();
    new_ctx->thread_count = core_count - FIBER_FREE_CORES;
    
    optio_array_create(new_ctx->raw_threads, new_ctx->thread_count);
    optio_array_resize(new_ctx->raw_threads, new_ctx->thread_count);
    
    optio_array_create(new_ctx->thread_local_data, new_ctx->thread_count);
    optio_array_resize(new_ctx->thread_local_data, new_ctx->thread_count);
    
    /* setup main thread */
    struct optio_thread *this_thread = optio_thread_create_this();
    {
      new_ctx->raw_threads[0] = this_thread;
      new_ctx->thread_local_data[0].worker_fiber = 0;
      new_ctx->raw_threads[0] = this_thread;
    }

    /* threads for the other cores */
    for(int i = 1; i < new_ctx->thread_count; ++i)
    {
      struct optio_thread_desc desc;
      desc.func = optio_internal_fiber_dispatcher;
      desc.arg = (void*)new_ctx;
      desc.affinity = i;
      
      optio_thread_create(&new_ctx->raw_threads[i], &desc);
    }

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
optio_dispatcher_destroy(struct optio_dispatcher_ctx **c)
{
  /* param assert */
  FIBER_ASSERT(c);
  
  (*c)->dispatch_state = FIBER_DISPATCHER_DESTROY;

  /* destroy threads */
  {
    int th_count = (*c)->thread_count;

    for(int i = 0; i < th_count; ++i)
    {
      optio_thread_destroy(&(*c)->raw_threads[i]);
    }
    
    optio_array_destroy((*c)->thread_local_data);
    optio_array_destroy((*c)->raw_threads);
  }
  
  optio_job_queue_destroy(&(*c)->job_queue);
  optio_fiber_pool_destroy(&(*c)->fiber_pool);
  
  FIBER_FREE(*c);
}


/* ------------------------------------------------ [ Dispatcher Actions ] -- */


void
optio_dispatcher_run(
  struct optio_dispatcher_ctx *c)
{
  /* param assert */
  {
    FIBER_ASSERT(c);
  }

  /* allow threads to start executing, and have this thread join in the fun */
  {
    c->dispatch_state = FIBER_DISPATCHER_RUNNING;
    
    optio_internal_fiber_dispatcher(c);
  }

  /* wait for threads to clean up */
  {
    int th_count = c->thread_count;
  
    optio_thread_join(c->raw_threads, th_count);
  }
    
  FIBER_LOG("Dispatch shutdown");
}


int
optio_dispatcher_add_jobs(
  struct optio_dispatcher_ctx *c,
  struct optio_job_desc *desc,
  int job_count)
{
  /* param assert */
  {
    FIBER_ASSERT(c);
    FIBER_ASSERT(desc);
    FIBER_ASSERT(job_count);
  }

  return optio_job_queue_add_batch(&c->job_queue, desc, job_count);
}


void
optio_dispatcher_wait_for_counter(
  struct optio_dispatcher_ctx *ctx,
  int counter)
{
  struct optio_thread **thds = ctx->raw_threads;
  const int thds_count = ctx->thread_count;
  const int thd_index = optio_thread_find_this(thds, thds_count);
  
  struct optio_thread_data *tls = &ctx->thread_local_data[thd_index];
  
  FIBER_ASSERT(tls);
  FIBER_ASSERT(tls->worker_fiber);

  optio_fiber_pool_block(&ctx->fiber_pool, tls->worker_fiber);
  
  struct optio_fiber *worker = tls->worker_fiber;
  optio_fiber_set_block_id(worker, counter);
  
  tls->worker_fiber = 0;
  
  optio_fiber_switch(worker, tls->home_fiber);
}


/* ------------------------------------------------- [ Dispatcher Config ] -- */


#undef FIBER_NO_RETURN
#undef FIBER_UNREACHABLE
