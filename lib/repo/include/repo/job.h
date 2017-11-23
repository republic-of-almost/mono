#ifndef JOB_INCLUDED_9D97A248_9945_4B77_B01A_C29B8236DA2B
#define JOB_INCLUDED_9D97A248_9945_4B77_B01A_C29B8236DA2B


/* --------------------------------------------------------------- [ Job ] -- */

typedef void(*repo_job_func)(int);

struct repo_job_desc
{
  repo_job_func job_func;
};


void      repo_job_submit(&repo_job_desc, int count);


#endif /* inc guard */
