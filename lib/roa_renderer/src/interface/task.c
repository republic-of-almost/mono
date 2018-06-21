#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <task/task.h>
#include <roa_math/math.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>
#include <stdio.h>
#include <roa_lib/spin_lock.h>
#include <string.h>


unsigned
roa_renderer_task_pump(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_task **tasks)
{
        if (roa_atomic_int_load(&ctx->tasks.curr_task_count) <= 0) {
               
                roa_spin_lock_aquire(&ctx->tasks.lock);

                /* flip task buffer */
                struct roa_renderer_task *old_curr = ctx->tasks.curr_tasks;

                ctx->tasks.curr_tasks = ctx->tasks.back_tasks;
                ctx->tasks.back_tasks = old_curr;

                roa_array_clear(old_curr);

                *tasks = ctx->tasks.curr_tasks;

                int task_count = (int)roa_array_size(ctx->tasks.curr_tasks);
                roa_atomic_int_store(&ctx->tasks.curr_task_count, task_count);

                roa_spin_lock_release(&ctx->tasks.lock);
        }

        return roa_atomic_int_load(&ctx->tasks.curr_task_count);;
}


void
roa_renderer_task_run(
  roa_renderer_ctx_t ctx,
	struct roa_renderer_task *task)
{
        ROA_ASSERT(task);

        roa_renderer_task_func func = task->func;

        if(func) {
                func(task->arg1, task->arg2, task->arg3);

                roa_atomic_int_dec(&ctx->tasks.curr_task_count);
        }
}
