#include <roa_lib/fundamental.h>
#include <roa_job/dispatcher.h>
#include <common/app_data.h>
#include <common/app_lifetime.h>
#include <roa_lib/spin_lock.h>


int
main(int argc, char **argv)
{
  /* command line args */
  ROA_UNUSED(argc);
  ROA_UNUSED(argv);

  /* application data */
  struct shorts_app_data app_data;
  ROA_MEM_ZERO(app_data);

  roa_spin_lock_init(&app_data.job_spin_lock);

  /* start job dispatcher */
  roa_dispatcher_create(&app_data.dispatcher_ctx, ROA_NULL);

  /* create inital job and submit */
  struct roa_job_desc job_desc;
  job_desc.func = app_startup;
  job_desc.arg = &app_data;
  job_desc.keep_on_calling_thread = ROA_TRUE;

  roa_dispatcher_add_jobs(
    app_data.dispatcher_ctx,
    &job_desc,
    1);

  /* start engines */
  roa_dispatcher_run(app_data.dispatcher_ctx);

  /* closing - bye! */
  return 0;
}