#include <common/app_lifetime.h>
#include <common/app_data.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>


ROA_JOB(app_frame, struct shorts_app_data*)
{
  /* call tickers */
  {
    roa_spin_lock_aquire(&arg->job_spin_lock);

    unsigned count = roa_array_size(arg->jobs_to_tick);

    if (count)
    {
      unsigned ticker_marker = roa_dispatcher_add_jobs(
        job_ctx,
        arg->jobs_to_tick,
        count);

      roa_dispatcher_wait_for_counter(job_ctx, ticker_marker);
    }

    roa_spin_lock_release(&arg->job_spin_lock);
  }

  /* submit new frame */
  if (roa_ctx_new_frame(arg->device_ctx))
  {    
    struct roa_job_desc frame_desc;
    frame_desc.arg = (void*)arg;
    frame_desc.func = app_frame;
    frame_desc.keep_on_calling_thread = ROA_TRUE;

    roa_dispatcher_add_jobs(
      job_ctx,
      &frame_desc,
      1);
  }
}


ROA_JOB(app_startup, struct shorts_app_data*)
{
  /* setup window context */
  roa_ctx_create(&arg->device_ctx);

  struct roa_ctx_window_desc win_desc;
  win_desc.title = "ROA Shorts";
  win_desc.width = 1200;
  win_desc.height = 720;

  roa_ctx_set_window_desc(arg->device_ctx, &win_desc);
  
  /* start frame */
  struct roa_job_desc frame_desc;
  frame_desc.arg = (void*)arg;
  frame_desc.func = app_frame;
  frame_desc.keep_on_calling_thread = ROA_TRUE;

  roa_dispatcher_add_jobs(
    job_ctx,
    &frame_desc,
    1);
}