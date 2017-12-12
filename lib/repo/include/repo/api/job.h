#ifndef JOB_API_INCLUDED_BF11EE49_C6E8_4874_A892_8387648FAF9E
#define JOB_API_INCLUDED_BF11EE49_C6E8_4874_A892_8387648FAF9E


#include <repo/job.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------- [ Job API ] -- */


struct repo_job_desc;
struct repo_engine;


//typedef unsigned(*repo_api_job_submit_fn)(struct repo_job_desc *desc, unsigned count);
//typedef void(*repo_api_job_wait_fn)(unsigned);
//typedef void(*repo_api_dispatcher_start_fn)(unsigned);
//
//
//struct repo_api_job
//{
//  repo_api_job_submit_fn        submit;
//  repo_api_job_wait_fn          wait;
//  repo_api_dispatcher_start_fn  dispatcher_start;
//  
//  void                          *user_data;
//};


int /* 1 if success */
repo_register_job_api(struct repo_engine *engine, struct repo_api_job job);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */
