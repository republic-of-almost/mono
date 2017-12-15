#include <stdint.h>
#include <assert.h>
#include <repo/repo.h>
#include <stdio.h>
#include <string.h>
#include <plugin/plugin.h>


struct repo_engine
{
  repo_api_job job_api;
  repo_api_window win_api;
};

repo_engine engine{};


void
test_job(void *)
{
  printf("hi\n");
}


unsigned
repo_job_submit_impl(struct repo_job_desc *desc, unsigned count)
{
  return engine.job_api.submit(desc, count);
}


int
repo_register_job_api_impl(struct repo_api_job *job)
{
  engine.job_api = *job;
  
  return 1;
}

/* window */


void
repo_window_get_desc_impl(struct repo_window_desc *out_desc)
{
  engine.win_api.window_get_desc(out_desc);
}

void
repo_window_set_desc_impl(const struct repo_window_desc *desc)
{
  engine.win_api.window_set_desc(desc);
}

int
repo_window_is_closing_impl()
{
  return engine.win_api.window_is_closing();
}

int
repo_register_window_api_impl(struct repo_api_window *win)
{
  engine.win_api = *win;
  
  return 1;
}

void
frame_job(void *)
{
  /* test image */
  
  if(!repo_window_is_closing_impl())
  {
    engine.win_api.window_process();
    
    /* submit another frame job */
    repo_job_desc desc[] = {
      {frame_job, nullptr},
    };
    
    repo_job_submit(desc, 1);
  }
}


typedef void(*repo_loader_fn)(void **fn, unsigned count);



const char *api_names[]
{
  /* win */
  "repo_window_get_desc",
  "repo_window_set_desc",
  "repo_window_is_closing",
  "repo_register_window_api",
  
  /* job */
  "repo_job_submit",
  "repo_register_job_api",
};

const void *api_funcs[] {
  /* win */
  (void*)repo_window_get_desc_impl,
  (void*)repo_window_set_desc_impl,
  (void*)repo_window_is_closing_impl,
  (void*)repo_register_window_api_impl,
  
  /* job */
  (void*)repo_job_submit_impl,
  (void*)repo_register_job_api_impl,
};


#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])


int
main()
{
  /* look for shared libs */
  {
    /* load shared libs */
    repo_plugins_load(api_names, api_funcs, ARRAY_SIZE(api_names));
  }

  /* create window */
  const repo_window_desc window_desc{
    "Repo",   /* title */
    800,      /* width */
    480,      /* height */
    0,        /* fullscreen */
  };

  repo_window_set_desc(&window_desc);
  engine.win_api.window_start_process();

  repo_job_desc desc[] = {
    {test_job, nullptr},
    {frame_job, nullptr},
  };
  
  repo_job_submit(desc, 2);

  engine.job_api.dispatcher_start();
  
  /* call startup on shared libs */ 
  {
    /* startup libs */
  }


  /* shutdown shared libs */
  {
    repo_plugins_unload();
  }
  return 0;
}
