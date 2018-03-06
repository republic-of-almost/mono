#include <rep/rep_api.h>
#include <data/engine_data.h>
#include <data/config.h>
#include <roa_lib/assert.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_job/dispatcher.h>


ROA_JOB(rep_app_tick, rep_task)
{
  ROA_BOOL new_frame = roa_ctx_new_frame(rep_data_ctx());

  if (new_frame == ROA_TRUE)
  {
    /* user task */
    {
      arg(0); /* test */
    }

    /* engine tasks */
    {
      
    }

    /* submit next frame */
    {
      struct roa_job_desc tick_desc;
      tick_desc.arg = arg;
      tick_desc.func = rep_app_tick;
      tick_desc.keep_on_calling_thread = ROA_TRUE;

      roa_dispatcher_add_jobs(rep_data_dispatcher(), &tick_desc, 1);
    }
  }

  return 0;
}


void
rep_app_create(
  struct rep_app_desc *desc)
{
  ROA_ASSERT(desc);

  rep_config_init();
  rep_data_init();

  if (desc)
  {
    /* window */
    struct roa_ctx_window_desc win_desc;
    win_desc.title = desc->title;
    win_desc.width = desc->width;
    win_desc.height = desc->height;

    roa_ctx_set_window_desc(rep_data_ctx(), &win_desc);

    /* setup job dispatcher */
    struct roa_job_desc tick_desc;
    tick_desc.arg = desc->frame_job;
    tick_desc.func = rep_app_tick;
    tick_desc.keep_on_calling_thread = ROA_TRUE;

    roa_dispatcher_add_jobs(rep_data_dispatcher(), &tick_desc, 1);
  }
}


void
rep_app_get(
  struct rep_app_desc * out_desc)
{
  ROA_ASSERT(out_desc);
}


void
rep_app_destroy()
{
  rep_data_destroy();
}