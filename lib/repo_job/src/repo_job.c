#include <repo/repo.h>
#include <optio/dispatcher.h>
#include <string.h>


/* -------------------------------------------------------------- [ Data ] -- */


struct optio_dispatcher_ctx *dispatcher_ctx;


/* --------------------------------------------------------------- [ Job ] -- */


struct job_wrapper
{
  repo_job_fn func;
  void        *argument;
};


#define JOB_COUNT 2048
struct job_wrapper jobs[JOB_COUNT];
struct optio_job_desc convert_desc[JOB_COUNT];


void wrap_func(struct optio_dispatcher_ctx *ctx, void *arg)
{
  struct job_wrapper *job = (struct job_wrapper*)arg;

  job->func(job->argument);

  /* free this slot */
  job->func = 0;
  job->argument = 0;
}


unsigned
job_submit(struct repo_job_desc *desc, unsigned count)
{
  /* mutex on jobs */
  
  unsigned job_index = 0;
  
  for (unsigned i = 0; i < count; ++i)
  {
    for(; job_index < JOB_COUNT; ++job_index)
    {
      if (jobs[job_index].func == 0)
      {
        jobs[job_index].func = desc[i].function;
        jobs[job_index].argument = desc[i].argument;
        
        convert_desc[i].func = wrap_func;
        convert_desc[i].arg = &jobs[job_index];
        
        break;
      }
    }
  }
  
  return optio_dispatcher_add_jobs(dispatcher_ctx, convert_desc, count);
}


void
job_wait(unsigned id)
{
  optio_dispatcher_wait_for_counter(dispatcher_ctx, id);
}


void
job_dispatcher_start()
{
  optio_dispatcher_run(dispatcher_ctx);
}


/* ------------------------------------------------------ [ Entry Points ] -- */


REPO_API void REPO_API_CALL
repo_module_create()
{
  /* clear data */
  memset(jobs, 0, sizeof(jobs));
  memset(convert_desc, 0, sizeof(convert_desc));

  /* create dispatcher */
  dispatcher_ctx = 0;
  optio_dispatcher_create(&dispatcher_ctx);
  
  /* register with repo api */
  struct repo_api_job job_api;
  
  job_api.submit           = job_submit;
  job_api.wait             = job_wait;
  job_api.dispatcher_start = job_dispatcher_start;
  job_api.user_data        = (void*)dispatcher_ctx;
  
  repo_register_job_api(job_api);
}


void
repo_module_destroy()
{
  /* destroy dispatcher */
  optio_dispatcher_destroy(&dispatcher_ctx);
}

