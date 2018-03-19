#ifndef THREAD_DATA_INCLUDED_79345474_A948_4A8F_9470_865C460DBEC8
#define THREAD_DATA_INCLUDED_79345474_A948_4A8F_9470_865C460DBEC8


#include <roa_lib/atomic.h>


struct fiber;
struct roa_job_desc;
struct job_batch;

struct thread_local_storage
{
  roa_atomic_int job_lock;
  /* array */ struct roa_job_desc *pending_jobs;

  /* array */ uint32_t *batch_ids;
  /* array */ struct job_batch *batches;
  uint32_t job_counter;

  roa_atomic_int fiber_lock;
  /* array */ struct fiber **free_fiber_pool;

  /* array */ uint32_t *blocked_fiber_batch_id;
  /* array */ struct fiber **blocked_fibers;

	struct fiber *executing_fiber;
	uint32_t executing_batch_id;
};


#endif /* inc guard */
