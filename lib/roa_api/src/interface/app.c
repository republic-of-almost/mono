#include <rep/rep_api.h>
#include <data/engine_data.h>
#include <data/config.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_job/dispatcher.h>
#include <renderer/renderer_tick.h>
#include <volt/volt.h>
#include <roa_lib/log.h>


rep_task application_frame_func = ROA_NULL;


ROA_JOB(rep_game_loop_tick, rep_task)
{
  ROA_BOOL new_frame = roa_ctx_new_frame(rep_data_ctx());

	ROA_LOG_INFO("frame %d", new_frame);

  if (new_frame == ROA_TRUE)
  {
    /* user task */
    {
      arg(0); /* test */

      roa_tagged_allocator_free(rep_config_tagged_hash_logic());
    }

    /* physics tasks */
    {
			/* physics is undecided atm */
      roa_tagged_allocator_free(rep_config_tagged_hash_physics());
    }

    /* renderer tasks */
    {
      struct roa_job_desc renderer_tick;
      renderer_tick.func = rep_renderer_tick;
      renderer_tick.arg = ROA_NULL;
      renderer_tick.keep_on_calling_thread = ROA_TRUE;

      unsigned marker = roa_dispatcher_add_jobs(
				rep_data_dispatcher(),
				&renderer_tick,
				1);

      roa_dispatcher_wait_for_counter(rep_data_dispatcher(), marker);

      roa_tagged_allocator_free(rep_config_tagged_hash_rendering());
    }
    
    /* submit next frame */
    {
      struct roa_job_desc tick_desc;
      tick_desc.arg										 = application_frame_func;
      tick_desc.func									 = rep_game_loop_tick;
      tick_desc.keep_on_calling_thread = ROA_TRUE;

      roa_dispatcher_add_jobs(rep_data_dispatcher(), &tick_desc, 1);
    }
  }
}


void
rep_app_create(
  const struct rep_app_desc *desc)
{
  ROA_ASSERT(desc);

  if (desc)
  {
    /* init order is important */
    roa_tagged_allocator_init();
    rep_config_init();
    rep_data_init();

    /* set desc */
    rep_app_set(desc);

    /* setup job dispatcher */
    struct roa_job_desc tick_desc;
    tick_desc.arg										 = application_frame_func;
    tick_desc.func									 = rep_game_loop_tick;
    tick_desc.keep_on_calling_thread = ROA_TRUE;

    roa_dispatcher_add_jobs(rep_data_dispatcher(), &tick_desc, 1);

    /* start dispatcher */
    roa_dispatcher_run(rep_data_dispatcher());

		ROA_LOG_INFO("No Jobs in dispatcher shutting down");

    /* closing */
    roa_tagged_allocator_destroy();
  }
}


void
rep_app_get(
  struct rep_app_desc * out_desc)
{
  /* param check */
  ROA_ASSERT(out_desc);

  if (out_desc)
  {
    struct roa_ctx_window_desc win_desc;
    roa_ctx_get_window_desc(rep_data_ctx(), &win_desc);

    out_desc->title = win_desc.title;
    out_desc->width = win_desc.width;
    out_desc->height = win_desc.height;
    out_desc->frame_job = application_frame_func;
  }
}


void
rep_app_set(
  const struct rep_app_desc * desc)
{
  /* param check */
  ROA_ASSERT(desc);

  if (desc)
  {
    /* window */
    struct roa_ctx_window_desc win_desc;
    win_desc.title = desc->title;
    win_desc.width = desc->width;
    win_desc.height = desc->height;

    roa_ctx_set_window_desc(rep_data_ctx(), &win_desc);

    /* global application function */
    application_frame_func = desc->frame_job;
  }
}


void
rep_app_destroy()
{
  rep_data_destroy();
}
