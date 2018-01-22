#ifndef DISPATCHER_INCLUDED_57EB7719_1AB4_4D4A_B8A1_0F2EC4CA6CAE
#define DISPATCHER_INCLUDED_57EB7719_1AB4_4D4A_B8A1_0F2EC4CA6CAE


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------- [ Dispatcher Types ] -- */


struct roa_dispatcher_ctx;


/* ----------------------------------------------- [ Dispatcher Lifetime ] -- */
/*
You must create a dispatcher context to run jobs on.

_example_

```c
roa_dispatcher_ctx *my_ctx = 0;
roa_dispatcher_create(&my_ctx);
```

and destroy it when you are done.

```c
roa_dispatcher_destroy(&my_ctx);
```
*/

struct roa_dispatcher_desc
{
  int free_cores;     /* -1 if you wish it to run on a single core */
  int max_fibers;     /* number of fibers that should be created */
  int max_jobs;       /* number of jobs that should be created */
};


void
roa_dispatcher_create(
  struct roa_dispatcher_ctx **c,          /* must be a valid pointer */
  const struct roa_dispatcher_desc *desc  /* optional */
);

void
roa_dispatcher_destroy(
  struct roa_dispatcher_ctx **c   /* must be a valid object */
);


/* ------------------------------------------------ [ Dispatcher Actions ] -- */
/*
  You will need to sumbit some jobs before running the application. This is
  split to allow you to stage some work before staring. Once you start running
  the dispatcher will continue until it has no more work todo then it will
  quit.

  If you wish to block a job until the submitted jobs are finished you can
  wait on the counter;
*/

typedef void(*roa_job_func)(struct roa_dispatcher_ctx *c, void *arg);

struct roa_job_desc
{
  roa_job_func func;                      /* valid function */
  void *arg;                                /* optional - can be on stack */
  int keep_on_calling_thread;               /* 1 / 0 keep on calling thread */
};


void
roa_dispatcher_run(
  struct roa_dispatcher_ctx *c            /* must be a valid context */
);


unsigned                                    /* returns a marker for the job batch */
roa_dispatcher_add_jobs(
  struct roa_dispatcher_ctx *c,           /* must be a valid context */
  struct roa_job_desc *desc,              /* array of job descriptions */
  int job_count                             /* number of descriptions */
);


void
roa_dispatcher_wait_for_counter(
  struct roa_dispatcher_ctx *c,           /* must be a valid context */
  unsigned marker                           /* must be a valid marker */
);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */
