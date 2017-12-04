#ifndef DISPATCHER_INCLUDED_57EB7719_1AB4_4D4A_B8A1_0F2EC4CA6CAE
#define DISPATCHER_INCLUDED_57EB7719_1AB4_4D4A_B8A1_0F2EC4CA6CAE


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------- [ Dispatcher Types ] -- */


struct optio_dispatcher_ctx;


/* ----------------------------------------------- [ Dispatcher Lifetime ] -- */
/*
  You must create a dispatcher context to run jobs on.
  
  _example_
  
  ```c
  optio_dispatcher_ctx *my_ctx = 0;
  optio_dispatcher_create(&my_ctx);
  ```
  
  and destroy it when you are done.
  
  ```c
  optio_dispatcher_destroy(&my_ctx);
  ```
*/

void
optio_dispatcher_create(
  struct optio_dispatcher_ctx **c   /* must be a valid pointer */
);
  
void
optio_dispatcher_destroy(
  struct optio_dispatcher_ctx **c   /* must be a valid object */
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

typedef void(*optio_job_func)(struct optio_dispatcher_ctx *c, void *arg);

struct optio_job_desc
{
  optio_job_func func;                      /* valid function */
  void *arg;                                /* optional - can be on stack */
  struct optio_job_atomic_counter *counter; /* hide this */
};


void
optio_dispatcher_run(
  struct optio_dispatcher_ctx *c            /* must be a valid context */
);


unsigned                                    /* returns a marker for the job batch */
optio_dispatcher_add_jobs(
  struct optio_dispatcher_ctx *c,           /* must be a valid context */
  struct optio_job_desc *desc,              /* array of job descriptions */
  int job_count                             /* number of descriptions */
);


void
optio_dispatcher_wait_for_counter(
  struct optio_dispatcher_ctx *c,           /* must be a valid context */
  unsigned marker                           /* must be a valid marker */
);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */
