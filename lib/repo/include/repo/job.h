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


struct repo_job_span
{
  repo_job_desc   *desc_array;
  unsigned        desc_count;
};


/* ----------------------------------------------------- [ Job Interface ] -- */


unsigned  /* returns your batch number */
repo_job_submit(struct repo_job_span span);


void
repo_job_submit_and_wait(struct repo_job_span span);


void
repo_job_wait(unsigned batch_id);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */
