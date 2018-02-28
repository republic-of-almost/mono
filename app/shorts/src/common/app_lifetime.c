#include <common/app_lifetime.h>
#include <common/app_data.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>
#include <roa_lib/assert.h>
#include <volt/volt.h>
#include <doors/door_interface.h> /* startup state */


ROA_JOB(app_frame, struct shorts_app_data*)
{
  ROA_ASSERT(arg);

  /* thinkers */
  {
    roa_spin_lock_aquire(&arg->think_lock);

    unsigned count = roa_array_size(arg->think_jobs);

    if (count)
    {
      unsigned ticker_marker = roa_dispatcher_add_jobs(
        job_ctx,
        arg->think_jobs,
        count);

      roa_dispatcher_wait_for_counter(job_ctx, ticker_marker);
    }

    roa_spin_lock_release(&arg->think_lock);
  }

  /* render */
  {
    roa_spin_lock_aquire(&arg->render_lock);

    unsigned count = roa_array_size(arg->render_jobs);

    if (count)
    {
      unsigned render_marker = roa_dispatcher_add_jobs(
        job_ctx,
        arg->render_jobs,
        count);

      roa_dispatcher_wait_for_counter(job_ctx, render_marker);
    }

    roa_spin_lock_release(&arg->render_lock);

    /* render cmd lists */
    volt_ctx_execute(arg->volt_ctx);
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
  ROA_ASSERT(arg);

  /* setup window context */
  roa_ctx_create(&arg->device_ctx);

  struct roa_ctx_window_desc win_desc;
  win_desc.title = "ROA Shorts";
  win_desc.width = 1200;
  win_desc.height = 720;

  roa_ctx_set_window_desc(arg->device_ctx, &win_desc);

  /* setup volt */
  volt_ctx_create(&arg->volt_ctx);
  
  /* start frame and inital state */
  struct roa_job_desc frame_desc[2];

  frame_desc[0].arg = (void*)arg;
  frame_desc[0].func = app_frame;
  frame_desc[0].keep_on_calling_thread = ROA_TRUE;

  frame_desc[1].arg = (void*)arg;
  frame_desc[1].func = door_startup;
  frame_desc[1].keep_on_calling_thread = ROA_FALSE;

  roa_dispatcher_add_jobs(
    job_ctx,
    ROA_ARRAY_PTR(frame_desc),
    ROA_ARRAY_COUNT(frame_desc));
}


ROA_JOB(app_add_thinker, struct app_thinker_data_arg*)
{
  ROA_ASSERT(arg);
  ROA_ASSERT(arg->app_data);

  roa_atomic_int *lock = &arg->app_data->think_lock;

  roa_spin_lock_aquire(lock);

  roa_array_push(arg->app_data->think_jobs, arg->desc);

  roa_spin_lock_release(lock);
}

ROA_JOB(app_add_renderer, struct app_renderer_data_arg*)
{
  ROA_ASSERT(arg);
  ROA_ASSERT(arg->app_data);

  roa_atomic_int *lock = &arg->app_data->render_lock;

  roa_spin_lock_aquire(lock);

  roa_array_push(arg->app_data->render_jobs, arg->desc);

  roa_spin_lock_release(lock);
}