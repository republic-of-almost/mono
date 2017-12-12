#include <repo/job.h>
#include <repo/api/job.h>
#include <string.h>


/* ----------------------------------------------------- [ Job Interface ] -- */


unsigned
repo_job_submit(struct repo_engine *engine, struct repo_job_desc *desc_array, unsigned count)
{
  return engine->job_api.submit(desc_array, count);
}


void
repo_job_submit_and_wait(struct repo_engine *engine, struct repo_job_desc *desc_array, unsigned count)
{
  
}


void
repo_job_wait(struct repo_engine *engine, unsigned batch_id)
{
}


/* ----------------------------------------------------------- [ Job API ] -- */


int
repo_register_job_api(struct repo_engine *engine, struct repo_api_job job)
{
  printf("Registered JOB API\n");
  
  engine->job_api = job;
  
//  job.submit(0, 0);
//  memcpy(&job_data, &job, sizeof(struct repo_api_job));
//  job_data.submit = job.submit;
  
//  job_data = job;
  
  return 0;
}
