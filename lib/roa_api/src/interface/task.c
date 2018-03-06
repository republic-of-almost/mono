#include <rep/rep_api.h>
#include <data/engine_data.h>
#include <roa_lib/assert.h>
#include <roa_job/dispatcher.h>


struct rep_task_wrapper_arg
{
  rep_task task_func;
  void *task_arg;
};


ROA_JOB(rep_task_wrapper, struct rep_task_wrapper_arg*)
{
  arg->task_func(arg->task_arg);
};


unsigned
rep_submit_tasks(
  struct rep_task_desc *tasks,
  unsigned count)
{
  /* param check */
  ROA_ASSERT(tasks);
  ROA_ASSERT(count);

  /* allocate space for args */
  struct rep_task_wrapper_arg *args;

  /* allocate space for desc */
  struct roa_job_desc *wrapped_desc;

  for (unsigned i = 0; i < count; ++i)
  {
    args[i].task_arg = tasks[i].arg;
    args[i].task_func = tasks[i].function;

    wrapped_desc[i].arg = &args[i];
    wrapped_desc[i].func = rep_task_wrapper;
    wrapped_desc[i].keep_on_calling_thread = ROA_FALSE;
  }

  return roa_dispatcher_add_jobs(rep_data_dispatcher(), wrapped_desc, count);
}


void
rep_wait_for_tasks(
  unsigned marker)
{
  /* param check */
  ROA_ASSERT(marker);

  roa_dispatcher_wait_for_counter(rep_data_dispatcher(), marker);
}