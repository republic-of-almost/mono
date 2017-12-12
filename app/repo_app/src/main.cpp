#include <optio/dispatcher.h>
#include <stdint.h>
#include <dlfcn.h>
#include <assert.h>
#include <repo/job.h>


typedef void(*repo_api_create)(struct repo_engine *);



void
test_job(void *arg)
{
  
}


int
main()
{
  repo_engine engine;

  /* look for shared libs */
  {
    /* load shared libs */
    void *handle = dlopen("/Users/PhilCK/Desktop/rep_of_a/output/development/libRepoJob.dylib", RTLD_NOW);
    assert(handle);
    
    repo_api_create create_fn = (repo_api_create)dlsym(handle, "repo_api_register");
    assert(create_fn);
    
    create_fn(&engine);
  }
  
  repo_job_desc desc[] = {
    {test_job, nullptr},
    {test_job, nullptr},
  };
  
  repo_job_submit(&engine, desc, 2);
  
  /* call startup on shared libs */ 
  {
    /* startup libs */
  }

  /* run application */
  {
    
  }

  /* shutdown shared libs */
  {
    /* shutdown each lib */
  }
  return 0;
}
