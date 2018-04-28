#include <roa_ctx/roa_ctx.h>
#include <roa_job/roa_job.h>
#include <roa_lib/alloc.h>
#include <roa_graph/roa_graph.h>
#include <data/config.h>
#include <roa_renderer/roa_renderer.h>
#include <rep/rep_api.h>


roa_ctx_t roa_ctx_data;

roa_job_dispatcher_ctx_t dispatcher;
struct roa_tagged_allocator dispatcher_allocator;

roa_graph_ctx_t roa_graph_ctx_data;

roa_renderer_ctx_t roa_renderer_ctx_data;

struct rep_input_desc rep_input_data;

void
rep_data_init()
{
  /* window and input */
  ROA_MEM_ZERO(roa_ctx_data);
  roa_ctx_create(&roa_ctx_data);

  /* fiber dispatcher */
  ROA_MEM_ZERO(dispatcher);
  roa_job_dispatcher_ctx_create(&dispatcher, ROA_NULL);

  ROA_MEM_ZERO(dispatcher_allocator);
  roa_tagged_allocator_create(&dispatcher_allocator, rep_config_tagged_hash_logic());

  /* graph */
	ROA_MEM_ZERO(roa_graph_ctx_data);
	roa_graph_ctx_create(&roa_graph_ctx_data);

  /* renderer */
  ROA_MEM_ZERO(roa_renderer_ctx_data);
  roa_renderer_ctx_create(&roa_renderer_ctx_data, ROA_NULL);

  /* input */
  ROA_MEM_ZERO(rep_input_data);
  rep_input_data.kb = roa_zalloc(sizeof(struct rep_keyboard_desc));
  rep_input_data.kb_count = 1;
  rep_input_data.kb[0].keys = roa_zalloc(sizeof(rep_keystate) * REP_KB_COUNT);

  rep_input_data.ms = roa_zalloc(sizeof(struct rep_mouse_desc));
  rep_input_data.ms_count = 1;
  rep_input_data.ms[0].ms_buttons = roa_zalloc(sizeof(rep_keystate) * REP_MS_COUNT);

  rep_input_data.gp = roa_zalloc(sizeof(struct rep_keyboard_desc) * 4);
  rep_input_data.gp_count = 4;
  rep_input_data.gp[0].gp_buttons = roa_zalloc(sizeof(rep_keystate) * REP_GP_COUNT);
  rep_input_data.gp[1].gp_buttons = roa_zalloc(sizeof(rep_keystate) * REP_GP_COUNT);
  rep_input_data.gp[2].gp_buttons = roa_zalloc(sizeof(rep_keystate) * REP_GP_COUNT);
  rep_input_data.gp[3].gp_buttons = roa_zalloc(sizeof(rep_keystate) * REP_GP_COUNT);
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


roa_job_dispatcher_ctx_t
rep_data_dispatcher()
{
  return dispatcher;
}


struct rep_input_desc*
rep_data_input_data()
{
  return &rep_input_data;
}


struct roa_tagged_allocator*
rep_data_dispatcher_allocator()
{
  return &dispatcher_allocator;
}


roa_graph_ctx_t
rep_data_graph()
{
	return roa_graph_ctx_data;
}


roa_renderer_ctx_t
rep_data_renderer()
{
  return roa_renderer_ctx_data;
}
