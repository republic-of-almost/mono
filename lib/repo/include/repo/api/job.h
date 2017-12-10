#ifndef JOB_API_INCLUDED_BF11EE49_C6E8_4874_A892_8387648FAF9E
#define JOB_API_INCLUDED_BF11EE49_C6E8_4874_A892_8387648FAF9E


#ifdef __cplusplus
extern "C" {
#endif


/* ----------------------------------------------------------- [ Job API ] -- */


typedef unsigned(*repo_api_job_submit)(struct repo_job_desc *desc, unsigned count);
typedef void(*repo_api_wait)(unsigned);

struct repo_api_job
{
  repo_api_job_submit submit;
  repo_api_job_wait wait;
};


int /* 1 if success */
repo_register_job_api(struct repo_api_job job);


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* inc guard */