#include <stdint.h>
#include <assert.h>
#include <repo/repo.h>
#include <stdio.h>
#include <string.h>
#include <plugin/plugin.h>


struct repo_engine
{
  repo_api_job job_api;
};

repo_engine engine{};


void
test_job(void *)
{
  printf("hi\n");
}


void
repo_job_submit_impl(struct repo_engine *e, struct repo_job_desc *desc, unsigned count)
{
  e->job_api.submit(desc, count);
}


int
repo_register_job_api_impl(struct repo_api_job job)
{
  engine.job_api = job;
  
  return 0;
}


typedef void(*repo_loader_fn)(void **fn, unsigned count);



const char *api_names[] {
  "repo_register_job_api",
};

void *api_funcs[] {
  (void*)repo_register_job_api_impl,
};


int
main()
{
  /* look for shared libs */
  {
    /* load shared libs */
    repo_plugins_load(api_names, api_funcs, 1);
  }
  
  repo_job_desc desc[] = {
    {test_job, nullptr},
    {test_job, nullptr},
  };
  
  repo_job_submit_impl(&engine, desc, 2);

  engine.job_api.dispatcher_start();
  
  /* call startup on shared libs */ 
  {
    /* startup libs */
  }

  /* run application */
  {
    
  }

  /* shutdown shared libs */
  {
    repo_plugins_unload();
  }
  return 0;
}
