#include <roa_job/roa_job.h>
#include <roa_renderer/roa_renderer.h>
#include <data/config.h>
#include <data/engine_data.h>
#include <roa_lib/alloc.h>


ROA_JOB(rep_renderer_task, struct roa_renderer_task *)
{
  ROA_UNUSED(job_ctx);

  roa_renderer_task_run(arg);
}


ROA_JOB(rep_renderer_tick, void*)
{
  struct roa_renderer_task **tasks = ROA_NULL;

  /* create an allocator here */
  struct roa_tagged_allocator allocator;

  roa_tagged_allocator_create(&allocator, rep_config_tagged_hash_rendering());

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
      desc[i].thread_locked = ROA_FALSE;
    }

    uint64_t marker = roa_job_submit(job_ctx, desc, count);
    roa_job_wait(job_ctx, marker);
  }

  roa_renderer_ctx_execute(rep_data_renderer());
}
