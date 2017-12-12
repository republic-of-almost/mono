#include <stdint.h>
#include <assert.h>
#include <repo/repo.h>
#include <stdio.h>
#include <string.h>

#ifndef _WIN32
#include <dlfcn.h>
#else
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#endif


struct repo_engine
{
  repo_api_job job_api;
};

repo_engine engine{};


void
test_job(void *arg)
{
  
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


void*
repo_api_loader_impl(const char *api_name)
{
  /* be super careful here */
  if(strcmp(api_name, "repo_register_job_api") == 0) { return (void*)repo_register_job_api_impl; }
  
  return 0;
}


int
main()
{
  /* look for shared libs */
  {
    /* load shared libs */
    #ifndef _WIN32
    void *handle = dlopen("/Users/PhilCK/Desktop/rep_of_a/output/development/libRepoJob.dylib", RTLD_NOW);
    assert(handle);
    
    repo_module_api_loader_fn reg_api = (repo_module_api_loader_fn)dlsym(handle, "repo_module_api_loader");
    
    reg_api(repo_api_loader_impl);
    
    repo_module_create_fn create_fn = (repo_module_create_fn)dlsym(handle, "repo_module_create");
    assert(create_fn);
    create_fn();
    #else
    HINSTANCE handle = LoadLibrary(_T("C:/Users/SimStim/Developer/mono/output/development/RepoJob.dll"));
    assert(handle);

    repo_module_api_loader_fn reg_api = (repo_module_api_loader_fn)GetProcAddress(handle, "repo_module_api_loader");
    repo_module_create_fn create_fn = (repo_module_create_fn)GetProcAddress(handle, "repo_module_create");

    reg_api(repo_api_loader_impl);
    create_fn();

    #endif
  }
  
  repo_job_desc desc[] = {
    {test_job, nullptr},
    {test_job, nullptr},
  };
  
  repo_job_submit_impl(&engine, desc, 2);
  
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
