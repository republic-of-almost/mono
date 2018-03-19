#ifndef THREAD_PROCESS_INCLUDED_7EF312E5_B2FF_4DAC_ABEA_BEE7CA47ED13
#define THREAD_PROCESS_INCLUDED_7EF312E5_B2FF_4DAC_ABEA_BEE7CA47ED13


void
fiber_process(void *arg);


typedef void* roa_thread_id;
struct roa_job_dispatcher_ctx;


/* pass an instance of this structure to the thread_process */
/* it will be freed by the process */
struct thread_arg
{
  roa_thread_id *roa_thread_id;
  struct roa_job_dispatcher_ctx *ctx;
};

void*
thread_process(void *arg);


int
job_internal_find_thread_index(struct roa_job_dispatcher_ctx *ctx);


#endif /* inc guard */