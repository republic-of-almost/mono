#include <roa_ctx/roa_ctx.h>
#include <roa_job/dispatcher.h>


roa_ctx_t roa_ctx_data;
roa_dispatcher_ctx_t roa_dispatcher_data;

void
rep_data_init()
{
  /* window and input */
  roa_ctx_data = ROA_NULL;
  roa_ctx_create(&roa_ctx_data);

  /* fiber dispatcher */
  roa_dispatcher_data = ROA_NULL;
  roa_dispatcher_create(&roa_dispatcher_data, ROA_NULL);
}


void
rep_data_destroy()
{
  roa_ctx_destroy(&roa_ctx_data);
}


roa_ctx_t
rep_data_ctx()
{
  return roa_ctx_data;
}


roa_dispatcher_ctx_t
rep_data_dispatcher()
{
  return roa_dispatcher_data;
}