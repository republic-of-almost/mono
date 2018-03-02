#include <doors/door_interface.h>
#include <doors/door_data/door_data.h>
#include <doors/door_renderer/door_renderer.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/hash.h>
#include <common/app_data.h>
#include <common/app_lifetime.h>
#include <roa_lib/alloc.h>


ROA_JOB(door_think, struct shorts_app_data*)
{
  ROA_UNUSED(job_ctx);
  ROA_UNUSED(arg);

  /* think about logn */
  
}


ROA_JOB(door_startup, struct shorts_app_data*)
{
  ROA_UNUSED(job_ctx);
  ROA_UNUSED(arg);

  /* allocate logn_data and assign to the job_desc thinker and renderer */
  struct door_data *data = roa_zalloc(sizeof(*data));
  data->volt_ctx = arg->volt_ctx;
  data->win_ctx = arg->device_ctx;

  uint64_t renderer_hash = roa_hash("renderer");

  roa_tagged_allocator_create(
    &data->level_data.render_allocator,
    renderer_hash);

  /* allow some things to start */
  {
    struct roa_job_desc startup[1];
    startup[0].arg = data;
    startup[0].func = door_renderer_create;
    startup[0].keep_on_calling_thread = ROA_FALSE;

    unsigned startup_marker = roa_dispatcher_add_jobs(
      job_ctx,
      ROA_ARR_DATA(startup),
      ROA_ARR_COUNT(startup));

    roa_dispatcher_wait_for_counter(job_ctx, startup_marker);
  }

  /* create descs callbacks */
  {
    struct roa_job_desc submit_callback_desc[2];

    submit_callback_desc[0].arg = data;
    submit_callback_desc[0].func = door_think;
    submit_callback_desc[0].keep_on_calling_thread = ROA_FALSE;

    submit_callback_desc[1].arg = data;
    submit_callback_desc[1].func = door_render;
    submit_callback_desc[1].keep_on_calling_thread = ROA_FALSE;

    struct roa_job_desc add_logn_desc[2];

    struct app_thinker_data_arg thinker_arg;
    thinker_arg.app_data = arg;
    thinker_arg.desc = submit_callback_desc[0];

    add_logn_desc[0].arg = &thinker_arg;
    add_logn_desc[0].func = app_add_thinker;
    add_logn_desc[0].keep_on_calling_thread = ROA_FALSE;

    struct app_renderer_data_arg renderer_arg;
    renderer_arg.app_data = arg;
    renderer_arg.desc = submit_callback_desc[1];

    add_logn_desc[1].arg = &renderer_arg;
    add_logn_desc[1].func = app_add_renderer;
    add_logn_desc[1].keep_on_calling_thread = ROA_FALSE;

    /* add thinker and renderer job to application */
    unsigned marker = roa_dispatcher_add_jobs(
      job_ctx,
      ROA_ARR_DATA(add_logn_desc),
      ROA_ARR_COUNT(add_logn_desc));

    /* need to wait because stack allocated variables */
    roa_dispatcher_wait_for_counter(job_ctx, marker);
  }
}