#include <roa_job/dispatcher.h>
#include <roa_renderer/roa_renderer.h>
#include <volt/volt.h>
#include <data/engine_data.h>
#include <roa_lib/alloc.h>


ROA_JOB(rep_renderer_task, struct roa_renderer_task *)
{
  roa_renderer_task_run(arg);
}


ROA_JOB(rep_renderer_tick, void*)
{
  struct roa_renderer_task **tasks = ROA_NULL;

  /* create an allocator here */
  struct roa_tagged_allocator allocator;
  roa_tagged_allocator_create(&allocator, "renderer");

  unsigned count;
  
  /* wrap renderer tasks in jobs and execute */
	while (count = roa_renderer_task_pump(rep_data_renderer(), tasks), count > 0)
  {
    unsigned bytes = sizeof(struct roa_job_desc*) * count;
    struct roa_job_desc *desc = roa_tagged_allocator_alloc(&allocator, bytes);
    unsigned i;

    for (i = 0; i < count; ++i)
    {
      desc[i].func = rep_renderer_task;
      desc[i].arg = tasks[i];
      desc[i].keep_on_calling_thread = ROA_FALSE;
    }

    unsigned marker = roa_dispatcher_add_jobs(job_ctx, desc, count);
    roa_dispatcher_wait_for_counter(job_ctx, marker);
  }

  roa_renderer_ctx_execute(rep_data_renderer());
	volt_ctx_execute(rep_data_volt());
}
