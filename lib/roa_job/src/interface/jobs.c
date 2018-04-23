#include <roa_job/roa_job.h>
#include <jobs/jobs.h>
#include <roa_lib/spin_lock.h>
#include <roa_lib/thread.h>
#include <roa_lib/assert.h>
#include <ctx/context.h>
#include <thread_dispatch/thread_local_storage.h>
#include <thread_dispatch/thread_process.h> /* todo remove this */
#include <roa_lib/alloc.h>
#include <roa_lib/array.h>
#include <fiber/fiber.h>


#ifndef ROA_JOB_DEBUG_OUTPUT
#define ROA_JOB_DEBUG_OUTPUT 0
#endif

#ifdef ROA_JOB_DEBUG_OUTPUT
#include <stdio.h>
#endif


uint64_t
roa_job_submit(
  roa_job_dispatcher_ctx_t ctx,
  struct roa_job_desc *desc,
  unsigned count)
{
  /* find this thread */
  struct thread_local_storage *tls = ROA_NULL;
  int th_index = job_internal_find_thread_index(ctx);
	{
    ROA_ASSERT(th_index > -1);
    tls = &ctx->tls[th_index];
  }

  /* queue jobs */
  uint64_t marker;
  {
    roa_spin_lock_aquire(&tls->job_lock);

    /* new batch */
    struct job_batch new_batch;
    new_batch.count = count;

    new_batch.counter = roa_zalloc(sizeof(*new_batch.counter));

		roa_atomic_int_store(new_batch.counter, (int)count);

    uint32_t new_batch_id = ++tls->batch_counter;

    marker = ROA_PACK3232((uint32_t)th_index, new_batch_id);

    roa_array_push(tls->batches, new_batch);
		roa_array_push(tls->batch_ids, new_batch_id);

    /*
		if(ROA_IS_ENABLED(ROA_JOB_DEBUG_OUTPUT))
		{
			printf("New Marker %d, Thread %d Counter %d \n", marker, th_index, new_batch_id);
		}
    */

    /* push work */
    unsigned j;

    for (j = 0; j < count; ++j)
    {
			struct job_internal job;
			job.desc = desc[j];
			/*job.batch_id = new_batch_id;*/
      job.counter = new_batch.counter;

      roa_array_push(tls->pending_jobs, job);
    }

    if (ROA_IS_ENABLED(ROA_JOB_DEBUG_OUTPUT))
    {
      unsigned pending_job_count = roa_array_size(tls->pending_jobs);
      printf("Pending Jobs %d \n", pending_job_count);
    }

    roa_spin_lock_release(&tls->job_lock);
  }

  return marker;
}


void
roa_job_wait(
  roa_job_dispatcher_ctx_t ctx,
  uint64_t job_marker)
{
	int thread_id = (int)ROA_LOWER_32_BITS(job_marker);
	uint32_t wait_on_batch_id = ROA_UPPER_32_BITS(job_marker);

	/* todo: good to check but thread_id negates the need to find index */

  /* push onto blocked queue with marker */
	int th_index = job_internal_find_thread_index(ctx);
	ROA_ASSERT(th_index > -1);
	ROA_ASSERT(th_index == thread_id);

	struct thread_local_storage *tls = &ctx->tls[th_index];
  struct roa_fiber *this_fiber = tls->executing_fiber.worker_fiber;
	ROA_ASSERT(this_fiber != ROA_NULL);

	/* lock fibers and push pending */
	{
		roa_spin_lock_aquire(&tls->fiber_lock);

		roa_array_push(tls->blocked_fibers, tls->executing_fiber);
		roa_array_push(tls->blocked_fiber_batch_ids, wait_on_batch_id);

		tls->executing_fiber.worker_fiber = ROA_NULL;

		roa_spin_lock_release(&tls->fiber_lock);
	}

  /* fiber switching */
  {
    /* switch back to home fiber ... */

    roa_fiber_switch(this_fiber, tls->home_fiber);

    /* ... back from sleeping fiber */
  }

}
