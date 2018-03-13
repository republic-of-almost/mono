#include <roa_ctx/roa_ctx.h>
#include <roa_job/dispatcher.h>
#include <roa_lib/alloc.h>
#include <roa_graph/roa_graph.h>
#include <data/config.h>
#include <volt/volt.h>


roa_ctx_t roa_ctx_data;

roa_dispatcher_ctx_t dispatcher;
struct roa_tagged_allocator dispatcher_allocator;

roa_graph_ctx_t roa_graph_ctx_data;

volt_ctx_t volt_ctx_data;


void
rep_data_init()
{
  /* window and input */
  ROA_MEM_ZERO(roa_ctx_data);
  roa_ctx_create(&roa_ctx_data);

  /* fiber dispatcher */
  ROA_MEM_ZERO(dispatcher);
  roa_dispatcher_create(&dispatcher, ROA_NULL);

  ROA_MEM_ZERO(dispatcher_allocator);
  roa_tagged_allocator_create(&dispatcher_allocator, rep_config_tagged_hash_logic());

  ROA_MEM_ZERO(volt_ctx_data);
  volt_ctx_create(&volt_ctx_data);

	ROA_MEM_ZERO(roa_graph_ctx_data);
	roa_graph_ctx_create(&roa_graph_ctx_data);
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
  return dispatcher;
}


struct roa_tagged_allocator*
rep_data_dispatcher_allocator()
{
  return &dispatcher_allocator;
}


volt_ctx_t
rep_data_volt()
{
  return volt_ctx_data;
}


roa_graph_ctx_t
rep_data_graph()
{
	return roa_graph_ctx_data;
}
