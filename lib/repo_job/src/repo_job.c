#include <repo_job/repo_job.h>
#include <repo/api/job.h>
#include <optio/dispatcher.h>
#include <printf.h>


/* -------------------------------------------------------------- [ Data ] -- */


struct optio_dispatcher_ctx *dispatcher_ctx;


/* --------------------------------------------------------------- [ Job ] -- */


unsigned
job_submit(struct repo_job_desc *desc, unsigned count)
{
  printf("submitting jobs\n");
  
  return 0;
}


void
job_wait(unsigned id)
{
  
}


void
job_start(unsigned id)
{
  
}


/* ------------------------------------------------------ [ Entry Points ] -- */


void
repo_api_register(struct repo_engine *engine)
{
  /* create dispatcher */
  dispatcher_ctx = 0;
  optio_dispatcher_create(&dispatcher_ctx);
  
  /* register with repo api */
  struct repo_api_job job_api;
  
  job_api.submit           = job_submit;
  job_api.wait             = job_wait;
  job_api.dispatcher_start = job_start;
  job_api.user_data        = (void*)dispatcher_ctx;
  
  repo_register_job_api(engine, job_api);
}


void
repo_api_think()
{

}


void
repo_api_destroy()
{

}
