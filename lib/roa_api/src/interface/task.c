#include <rep/rep_api.h>
#include <data/engine_data.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_job/dispatcher.h>


/* --------------------------------------------- [ Internal Task Wrapper ] -- */


struct rep_task_wrapper_arg
{
  rep_task task_func;
  void *task_arg;
};


void
rep_task_wrapper(roa_dispatcher_ctx_t job_ctx, void *void_arg)
{
  ROA_UNUSED(job_ctx);

  struct rep_task_wrapper_arg *arg = (struct rep_task_wrapper_arg*)void_arg;
  arg->task_func(arg->task_arg);
};


/* -------------------------------------------------------------- [ Task ] -- */


unsigned
rep_task_submit(
  struct rep_task_desc *tasks,
  unsigned count)
{
  /* param check */
  ROA_ASSERT(tasks);
  ROA_ASSERT(count);

  /* allocate space for args */
  struct rep_task_wrapper_arg *args = ROA_NULL;
  args = roa_tagged_allocator_alloc(rep_data_dispatcher_allocator(), sizeof(*args) * count);
  ROA_ASSERT(args);

  /* allocate space for desc */
  struct roa_job_desc *wrapped_desc = ROA_NULL;
  wrapped_desc = roa_tagged_allocator_alloc(rep_data_dispatcher_allocator(), sizeof(*wrapped_desc) * count);
  ROA_ASSERT(wrapped_desc);

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
rep_task_wait(
  unsigned marker)
{
  /* param check */
  ROA_ASSERT(marker);

  roa_dispatcher_wait_for_counter(rep_data_dispatcher(), marker);
}