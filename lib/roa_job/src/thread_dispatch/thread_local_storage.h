#ifndef THREAD_DATA_INCLUDED_79345474_A948_4A8F_9470_865C460DBEC8
#define THREAD_DATA_INCLUDED_79345474_A948_4A8F_9470_865C460DBEC8


#include <roa_lib/atomic.h>
#include <jobs/jobs.h>


struct roa_fiber;
struct job_batch;


struct executing_fiber
{
  struct roa_fiber *worker_fiber;
  struct job_internal desc;
};


struct thread_local_storage
{
  roa_atomic_int job_lock;
  /* array */ struct job_internal *pending_jobs;

  /* array */ uint32_t *batch_ids;
  /* array */ struct job_batch *batches;
  uint32_t batch_counter;

  roa_atomic_int fiber_lock;
  /* array */ struct roa_fiber **free_fiber_pool;

  /* array */ uint32_t *blocked_fiber_batch_ids;
  /* array */ struct executing_fiber *blocked_fibers;

	struct executing_fiber executing_fiber;
  struct roa_fiber *home_fiber;
};


#endif /* inc guard */
