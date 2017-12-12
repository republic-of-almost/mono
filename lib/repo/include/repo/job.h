#ifndef JOB_INCLUDED_9D97A248_9945_4B77_B01A_C29B8236DA2B
#define JOB_INCLUDED_9D97A248_9945_4B77_B01A_C29B8236DA2B


#ifdef __cplusplus
extern "C" {
#endif


/* --------------------------------------------------------- [ Job Types ] -- */


typedef void(*repo_job_func)(void *arg);


struct repo_job_desc
{
  repo_job_func function;
  void          *argument;
};


typedef unsigned(*repo_api_job_submit_fn)(struct repo_job_desc *desc, unsigned count);
typedef void(*repo_api_job_wait_fn)(unsigned);
typedef void(*repo_api_dispatcher_start_fn)(unsigned);


struct repo_api_job
{
  repo_api_job_submit_fn        submit;
  repo_api_job_wait_fn          wait;
  repo_api_dispatcher_start_fn  dispatcher_start;
  
  void                          *user_data;
};



struct repo_engine
{
  struct repo_api_job job_api;
};


/* ----------------------------------------------------- [ Job Interface ] -- */


unsigned  /* returns your batch number */
repo_job_submit(struct repo_engine *engine, struct repo_job_desc *desc_array, unsigned count);


void
repo_job_submit_and_wait(struct repo_engine *engine, struct repo_job_desc *desc_array, unsigned count);


void
repo_job_wait(struct repo_engine *engine, unsigned batch_id);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */
