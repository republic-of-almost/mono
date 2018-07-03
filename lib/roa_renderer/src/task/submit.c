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
                struct roa_renderer_task cam_task;
                ROA_MEM_ZERO(cam_task);
                cam_task.func = task_camera_mats;
                cam_task.arg1 = arg1;
                cam_task.arg2 = &ctx->renderer_desc.camera_passes[i];
          
                roa_array_push(ctx->tasks.back_tasks, cam_task);
          
                struct roa_renderer_task decal_dec_task;
                ROA_MEM_ZERO(decal_dec_task);
                decal_dec_task.func = task_decal_decay;
                decal_dec_task.arg1 = arg1;
                decal_dec_task.arg2 = &ctx->renderer_desc.camera_passes[i];
          
                roa_array_push(ctx->tasks.back_tasks, decal_dec_task);
        }
}
