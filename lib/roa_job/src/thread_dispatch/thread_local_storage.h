#ifndef THREAD_DATA_INCLUDED_79345474_A948_4A8F_9470_865C460DBEC8
#define THREAD_DATA_INCLUDED_79345474_A948_4A8F_9470_865C460DBEC8


#include <roa_lib/atomic.h>
#include <roa_lib/signal.h>
#include <jobs/jobs.h>


#ifndef ROA_JOB_CACHELINE_PADDING
#define ROA_JOB_CACHELINE_PADDING 1
#endif


struct roa_fiber;
struct job_batch;


struct executing_fiber
{
  struct roa_fiber *worker_fiber;
  struct job_internal desc;
};


typedef enum _thread_state
{
  TLS_RUNNING,
  TLS_QUIT,
} thread_state;


struct thread_local_storage
{
  int thread_status;

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

  #if ROA_JOB_CACHELINE_PADDING == 1
  char padding[64];
  #endif
};


#endif /* inc guard */
