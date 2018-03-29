#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <task/task.h>
#include <stdio.h>


unsigned
roa_renderer_task_pump(
	roa_renderer_ctx_t ctx,
	struct roa_renderer_task **tasks)
{
  /* what stage are we at */
  static int i = 0;
  i += 1;

  if(i == 1)
  {

    /* cull objects per camera */
    {
      unsigned cam_count = roa_array_size(ctx->camera);
      unsigned i;

      for (i = 0; i < cam_count; ++i)
      {
        struct roa_renderer_task task;
        task.arg = ROA_NULL;
        task.func = task_sphere_culling;

        roa_array_push(ctx->tasks, task);
      }


      unsigned task_count = roa_array_size(ctx->tasks);

      if (task_count)
      {
        *tasks = ctx->tasks;
      }

      return task_count;;
    }

  }

  /* renderpass per camera */
	
	return 0;
}


void
roa_renderer_task_run(
	struct roa_renderer_task *task)
{
  if(task && task->func)
  {
    roa_renderer_task_func task_func = (roa_renderer_task_func)task->func;
    task_func(task->arg);
  }
}
