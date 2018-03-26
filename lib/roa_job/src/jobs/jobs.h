#ifndef JOBS_INCLDUED_7FA641F2_CAD6_4AD8_A406_E03E1FD4F2BC
#define JOBS_INCLDUED_7FA641F2_CAD6_4AD8_A406_E03E1FD4F2BC


#include <roa_lib/fundamental.h>
#include <roa_job/roa_job.h>
#include <roa_lib/atomic.h>


struct job_batch
{
  int count;
	roa_atomic_int *counter;
};


struct job_internal
{
	struct roa_job_desc desc;
	/* uint32_t batch_id; */
  roa_atomic_int *counter;
};


#endif /* inc guard */
