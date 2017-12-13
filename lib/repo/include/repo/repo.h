#ifndef REPO_API_INCLUDED_023742B6_4DD0_47C7_9B74_1824AD22B35F
#define REPO_API_INCLUDED_023742B6_4DD0_47C7_9B74_1824AD22B35F


#ifdef __cplusplus
extern "C" {
#endif


#ifdef _WIN32
#define REPO_API_CALL __cdecl
#define REPO_DLL_EXPORT __declspec(dllexport)
#else
#define REPO_API_CALL
#define REPO_DLL_EXPORT static
#endif


/* ------------------------------------------------------ [ Module Hooks ] -- */


typedef void(REPO_API_CALL *repo_module_create_fn)();
typedef void(REPO_API_CALL *repo_module_destroy_fn)();

typedef void*(*repo_api_loader_fn)(const char *name);
typedef void(REPO_API_CALL *repo_module_api_loader_fn)(repo_api_loader_fn loader);


/* --------------------------------------------------- [ Repo Window API ] -- */


struct repo_window_desc
{
  const char *name;
  unsigned width;
  unsigned height;
  unsigned fullscreen;
};

typedef void(*repo_window_get_desc_fn)(struct repo_window_desc *desc);
typedef void(*repo_window_set_desc_fn)(const struct repo_window_desc *desc);

static repo_window_get_desc_fn repo_window_get_desc;
static repo_window_set_desc_fn repo_window_set_desc;


/* ---------------------------------------------- [ Repo Window API Impl ] -- */


typedef void(*repo_api_window_process_fn)();


struct repo_api_window
{
  repo_api_window_process_fn window_process;
};

typedef void(*repo_register_window_api_fn)(struct repo_api_window *window);

static repo_register_window_api_fn repo_register_window_api;


/* ------------------------------------------------------ [ Repo Job API ] -- */

typedef void (*repo_job_fn)(void *arg);

struct repo_job_desc
{
  repo_job_fn   function;
  void          *argument;
};


typedef void (*repo_job_submit_fn)(struct repo_job_desc *desc, unsigned count);
typedef void (*repo_job_submit_and_wait_fn)(struct repo_job_desc *desc_array, unsigned count);
typedef void (*repo_job_wait_fn)(unsigned batch_id);


static repo_job_submit_fn          repo_job_submit;
static repo_job_submit_and_wait_fn repo_job_submit_and_wait;
static repo_job_wait_fn            repo_job_wait;


/* ------------------------------------------------- [ Repo Job API Impl ] -- */


typedef unsigned(*repo_api_job_submit_fn)(struct repo_job_desc *desc, unsigned count);
typedef void(*repo_api_job_wait_fn)(unsigned);
typedef void(*repo_api_dispatcher_start_fn)();

struct repo_api_job
{
  repo_api_job_submit_fn        submit;
  repo_api_job_wait_fn          wait;
  repo_api_dispatcher_start_fn  dispatcher_start;
  
  void                          *user_data;
};

typedef int(*repo_register_job_api_fn)(struct repo_api_job job);

static repo_register_job_api_fn repo_register_job_api;


/* --------------------------------------------------- [ Repo API Loader ] -- */
/*
  When the module is loaded up the functions get resolved.
*/

__declspec(dllexport) void __cdecl
repo_module_api_loader(repo_api_loader_fn loader)
//{
//  /* window */
//  repo_window_get_desc     = (repo_window_get_desc_fn)loader("repo_window_get_desc");
//  repo_window_set_desc     = (repo_window_set_desc_fn)loader("repo_window_set_desc");
//  repo_register_window_api = (repo_register_window_api_fn)loader("repo_register_window_api");
//
//  /* job */
//  repo_job_submit          = (repo_job_submit_fn)loader("repo_job_submit");
//  repo_job_submit_and_wait = (repo_job_submit_and_wait_fn)loader("repo_job_submit_and_wait");
//  repo_job_wait            = (repo_job_wait_fn)loader("repo_job_wait");
//  repo_register_job_api    = (repo_register_job_api_fn)loader("repo_register_job_api");
//}


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
