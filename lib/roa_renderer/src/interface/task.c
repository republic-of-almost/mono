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
  if(ctx->render == ROA_TRUE)
  {
    return 0;
  }

  roa_array_clear(ctx->tasks);

  unsigned cam_count = roa_array_size(ctx->camera);
  unsigned i;

  for(i = 0; i < cam_count; ++i)
  {
    struct roa_renderer_task render_task;
    render_task.func = task_render; 
    render_task.arg = ctx;

    roa_array_push(ctx->tasks, render_task);
  }

  ctx->render = ROA_TRUE;

  *tasks = ctx->tasks;
  return cam_count;
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
