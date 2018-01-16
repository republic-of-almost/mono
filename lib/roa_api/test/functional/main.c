/* main.c */
#include <roa/roa_api.h>
#include <roa_lib/assert.h>


ROA_JOB(test_job1, void*)
{
  ROA_ASSERT(arg == ROA_NULL);

  for (int i = 0; i < 1000000; ++i)
  {
    /* do nothing */
  }
}


ROA_JOB(test_job2, void*)
{
  ROA_ASSERT(arg == ROA_NULL);

  for (int i = 0; i < 1000; ++i)
  {
    /* do nothing */
  }
}


ROA_JOB(test_job3, void*)
{
  ROA_ASSERT(arg == ROA_NULL);

  for (int i = 0; i < 1000000; ++i)
  {
    /* do nothing */
  }
}


ROA_JOB(main_frame, void*)
{
  ROA_ASSERT(arg == ROA_NULL);

  struct roa_job_desc job_desc[3];
  job_desc[0].function = test_job1;
  job_desc[0].argument = ROA_NULL;
  job_desc[0].lock_to_thread = 0;

  job_desc[1].function = test_job2;
  job_desc[1].argument = ROA_NULL;
  job_desc[1].lock_to_thread = 0;

  job_desc[2].function = test_job3;
  job_desc[2].argument = ROA_NULL;
  job_desc[2].lock_to_thread = 0;

  roa_job_batch batch = roa_job_batch_submit(job_desc, 2);
  roa_job_batch_wait(batch);
}

int
main()
{
  struct roa_application_desc desc = {
    "ROA API Functional Test",
    1200,
    700,
    ROA_FALSE,
    main_frame,
    ROA_NULL,
  };

  roa_application_create(&desc);

  return 0;
}