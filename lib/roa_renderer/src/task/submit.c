#include <task/task.h>
#include <ctx/ctx.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>


void
task_submit(
        void *arg1,
        void *arg2,
        void *arg3)
{
        /* param check */
        ROA_ASSERT(arg1);
        ROA_UNUSED(arg2);
        ROA_UNUSED(arg3);

        /* create task for each camera */
        struct roa_renderer_ctx *ctx = (struct roa_renderer_ctx*)arg1;
  
        int cam_count = (int)roa_array_size(ctx->renderer_desc.camera_passes);

        int i;
        for(i = 0; i < cam_count; ++i) {
                struct roa_renderer_task task;
                task.func = task_camera_mats;
                task.arg1 = arg1;
                task.arg2 = &ctx->renderer_desc.camera_passes[i];
                task.arg3 = 0;

                roa_array_push(ctx->tasks.back_tasks, task);
        }
}
