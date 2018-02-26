#include <logn/logn_interface.h>
#include <roa_lib/fundamental.h>
#include <common/app_data.h>
#include <common/app_lifetime.h>


ROA_JOB(logn_think, struct shorts_app_data*)
{
  ROA_UNUSED(job_ctx);
  ROA_UNUSED(arg);


  /* think about logn */
}


ROA_JOB(logn_render, struct shorts_app_data*)
{
  ROA_UNUSED(job_ctx);
  ROA_UNUSED(arg);


  /* render data */
}



ROA_JOB(logn_startup, struct shorts_app_data*)
{
  ROA_UNUSED(job_ctx);
  ROA_UNUSED(arg);

  /* allocate logn_data and assign to the job_desc thinker and renderer */

  /* create descs callbacks */
  struct roa_job_desc submit_callback_desc[2];

  submit_callback_desc[0].arg = arg;
  submit_callback_desc[0].func = logn_think;
  submit_callback_desc[0].keep_on_calling_thread = ROA_FALSE;

  submit_callback_desc[1].arg = arg;
  submit_callback_desc[1].func = logn_render;
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

  int i = 1;

  int b = 2;
}