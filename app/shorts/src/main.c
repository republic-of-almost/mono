#include <roa_lib/fundamental.h>
#include <roa_lib/array.h>
#include <roa_job/roa_job.h>
#include <roa_lib/alloc.h>
#include <common/app_data.h>
#include <common/app_lifetime.h>
#include <roa_lib/spin_lock.h>


int
main(int argc, char **argv)
{
  /* command line args */
  ROA_UNUSED(argc);
  ROA_UNUSED(argv);

  /* allocators before anything */
  roa_tagged_allocator_init();

  /* application data */
  struct shorts_app_data app_data;
  ROA_MEM_ZERO(app_data);

  /* initalize app data */
  roa_spin_lock_init(&app_data.think_lock);
  roa_spin_lock_init(&app_data.render_lock);
  roa_array_create(app_data.render_jobs, 32);
  roa_array_create(app_data.think_jobs, 32);

  /* start job dispatcher */
  roa_job_dispatcher_ctx_create(&app_data.dispatcher_ctx, ROA_NULL);

  /* create inital job and submit */
  struct roa_job_desc job_desc;
  job_desc.func = app_startup;
  job_desc.arg = &app_data;
  job_desc.thread_locked = ROA_TRUE;

  roa_job_submit(
    app_data.dispatcher_ctx,
    &job_desc,
    1);

  /* start engines */
  roa_job_dispatcher_ctx_run(app_data.dispatcher_ctx);

  /* closing - bye! */
  roa_tagged_allocator_destroy();
  roa_job_dispatcher_ctx_destroy(&app_data.dispatcher_ctx);

  return 0;
}