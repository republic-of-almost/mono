#include <roa/roa_api.h>
#include <roa_lib/assert.h>
#include <interface/context.h>
#include <optio/dispatcher.h>


roa_job_batch
roa_job_batch_submit(
  struct roa_job_desc *desc,
  unsigned count)
{
  struct roa_context *ctx = roa_ctx();

  ROA_ASSERT(desc != ROA_NULL);
  ROA_ASSERT(count > 0);
  ROA_ASSERT(ctx != ROA_NULL);
  ROA_ASSERT(ctx->optio_ctx != ROA_NULL);
  ROA_ASSERT(sizeof(desc[0]) == sizeof(struct optio_job_desc));

  if(desc && count && ctx)
  {
    return optio_dispatcher_add_jobs(
      ctx->optio_ctx,
      (struct optio_job_desc*)desc,
      count);
  }

  return 0;
}


void
roa_job_batch_wait(
  roa_job_batch batch)
{
  struct roa_context *ctx = roa_ctx();

  ROA_ASSERT(batch);
  ROA_ASSERT(ctx != ROA_NULL);
  ROA_ASSERT(ctx->optio_ctx != ROA_NULL);

  if (batch && ctx)
  {
    optio_dispatcher_wait_for_counter(
      ctx->optio_ctx,
      (unsigned)batch);
  }
}