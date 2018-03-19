#include <roa_job/roa_job.h>
#include <jobs/jobs.h>
#include <roa_lib/spin_lock.h>
#include <roa_lib/thread.h>
#include <roa_lib/assert.h>
#include <ctx/context.h>
#include <thread_dispatch/thread_local_storage.h>
#include <roa_lib/array.h>


int
job_internal_find_thread_index(struct roa_job_dispatcher_ctx *ctx)
{
	roa_thread_id this_th = roa_thread_get_current_id();

  unsigned i;
	int th_index = -1;

  for (i = 0; i < ctx->thread_count; ++i)
  {
		if (ctx->thread_ids[i] == this_th)
    {
			th_index = (int)i;
      break;
    }
  }

	return th_index;
}


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
		roa_atomic_int_store(&new_batch.counter, (int)count);

    marker = ROA_PACK3232((uint32_t)th_index, tls->job_counter);

		uint32_t new_batch_id = ++tls->job_counter;

    roa_array_push(tls->batches, new_batch);
		roa_array_push(tls->batch_ids, new_batch_id);

    /* push work */
    unsigned j;

    for (j = 0; j < count; ++j)
    {
			struct job_internal job;
			job.desc = desc[j];
			job.batch_id = new_batch_id; 

      roa_array_push(tls->pending_jobs, desc[j]);
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
	int thread_id = (int)ROA_UPPER_32_BITS(job_marker);
	uint32_t batch_id = ROA_LOWER_32_BITS(job_marker);

	/* todo: good to check but thread_id negates the need to find index */

  /* push onto blocked queue with marker */
	int th_index = job_internal_find_thread_index(ctx);
	ROA_ASSERT(th_index > -1);
	ROA_ASSERT(th_index == thread_id);

	struct thread_local_storage *tls = &ctx->tls[th_index];
	ROA_ASSERT(tls->executing_fiber != ROA_NULL);

	/* lock fibers and push pending */
	{
		roa_spin_lock_aquire(&tls->fiber_lock);

		roa_array_push(tls->blocked_fibers, tls->executing_fiber);
		roa_array_push(tls->blocked_fiber_batch_id, batch_id);

		tls->executing_fiber = ROA_NULL;

		roa_spin_lock_release(&tls->fiber_lock);
	}
}
