#include <roa_job/roa_job.h>
#include <jobs/jobs.h>
#include <roa_lib/spin_lock.h>
#include <roa_lib/thread.h>
#include <roa_lib/assert.h>
#include <ctx/context.h>
#include <thread_dispatch/thread_local_storage.h>
#include <roa_lib/array.h>


uint64_t
roa_job_submit(
  roa_job_dispatcher_ctx_t ctx,
  struct roa_job_desc *desc,
  unsigned count)
{
  /* find this thread */
  struct thread_local_storage *tls = ROA_NULL;
  int th_index = -1;
  {
    roa_thread_id this_th = roa_thread_get_current_id();

    int i;

    for (i = 0; i < ctx->thread_count; ++i)
    {
      if (ctx->thread_ids[i] == this_th)
      {
        th_index = i;
        break;
      }
    }

    ROA_ASSERT(th_index > -1);
    tls = &ctx->tls[th_index];
  }


  /* queue jobs */
  uint64_t marker;
  {
    roa_spin_lock_aquire(&tls->job_lock);

    /* new batch */
    struct job_batch new_batch;
    new_batch.batch_id = ++tls->job_counter;
    new_batch.count = count;

    marker = ROA_PACK3232((uint32_t)th_index, tls->job_counter);

    roa_array_push(tls->batches, new_batch);

    /* push work */
    int j;

    for (j = 0; j < count; ++j)
    {
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
  /* push onto blocked queue with marker */
}