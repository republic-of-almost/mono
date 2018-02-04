#ifndef DISPATCHER_INCLUDED_57EB7719_1AB4_4D4A_B8A1_0F2EC4CA6CAE
#define DISPATCHER_INCLUDED_57EB7719_1AB4_4D4A_B8A1_0F2EC4CA6CAE


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------- [ Dispatcher Types ] -- */


typedef struct roa_dispatcher_ctx * roa_dispatcher_ctx_t;


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
  roa_dispatcher_ctx_t *c,          /* must be a valid pointer */
  const struct roa_dispatcher_desc *desc  /* optional */
);

void
roa_dispatcher_destroy(
  roa_dispatcher_ctx_t *c   /* must be a valid object */
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

typedef void(*roa_job_func)(roa_dispatcher_ctx_t c, void *arg);


#define ROA_JOB_DECL(job_name)                                \
void job_name(roa_dispatcher_ctx_t job_ctx, void *void_arg);            \

#define ROA_JOB(job_name, arg_type)                                     \
void job_wrap_##job_name##(roa_dispatcher_ctx_t job_ctx, arg_type arg); \
                                                                        \
void                                                                    \
job_name(roa_dispatcher_ctx_t job_ctx, void *void_arg)                  \
{                                                                       \
  job_wrap_##job_name##(job_ctx, (arg_type)void_arg);                   \
}                                                                       \
                                                                        \
void job_wrap_##job_name##(roa_dispatcher_ctx_t job_ctx, arg_type arg)

struct roa_job_desc
{
  roa_job_func func;                        /* valid function */
  void *arg;                                /* optional - can be on stack */
  int keep_on_calling_thread;               /* 1 / 0 keep on calling thread */
};


void
roa_dispatcher_run(
  roa_dispatcher_ctx_t c              /* must be a valid context */
);


unsigned                              /* returns a marker for the job batch */
roa_dispatcher_add_jobs(
  roa_dispatcher_ctx_t c,             /* must be a valid context */
  const struct roa_job_desc *desc,    /* array of job descriptions */
  int job_count                       /* number of descriptions */
);


void
roa_dispatcher_wait_for_counter(
  roa_dispatcher_ctx_t c,             /* must be a valid context */
  unsigned marker                           /* must be a valid marker */
);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */
