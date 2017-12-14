#include <repo/repo.h>

/* window */
repo_window_get_desc_fn     repo_window_get_desc;
repo_window_set_desc_fn     repo_window_set_desc;

repo_register_window_api_fn repo_register_window_api;

/* jobs */
repo_job_submit_fn          repo_job_submit;
repo_job_submit_and_wait_fn repo_job_submit_and_wait;
repo_job_wait_fn            repo_job_wait;

repo_register_job_api_fn    repo_register_job_api;


/* loader  */
REPO_API void REPO_API_CALL
repo_module_api_loader(repo_api_loader_fn loader)
{
  if(loader)
  {
    /* window */
    repo_window_get_desc = (repo_window_get_desc_fn)loader("repo_window_get_desc");
    repo_window_set_desc = (repo_window_set_desc_fn)loader("repo_window_set_desc");

    repo_register_window_api = (repo_register_window_api_fn)loader("repo_register_window_api");

    /* job */
    repo_job_submit          = (repo_job_submit_fn)loader("repo_job_submit");
    repo_job_submit_and_wait = (repo_job_submit_and_wait_fn)loader("repo_job_submit_and_wait");
    repo_job_wait            = (repo_job_wait_fn)loader("repo_job_wait");

    repo_register_job_api    = (repo_register_job_api_fn)loader("repo_register_job_api");
  }
}
