#include <task/task.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <roa_math/math.h>
#include <string.h>


void
task_camera_mats(void *arg1, void *arg2, void *arg3)
{
        /* param check */
        ROA_ASSERT(arg1);
        ROA_ASSERT(arg2);
        ROA_UNUSED(arg3);
  
        /* args */
        struct roa_renderer_ctx *ctx = (struct roa_renderer_ctx*)arg1;
        struct renderpass *rp = (struct renderpass*)arg2;

        struct roa_renderer_camera cam = rp->camera;

        /* view mat */
        roa_mat4 view;
        roa_float3 pos = roa_float3_import(cam.position);
        roa_float3 up = roa_float3_import(cam.up);
        roa_float3 at = roa_float3_import(cam.lookat);

        roa_mat4_lookat(&view, pos, at, up);
        roa_mat4_export(&view, rp->camera_view);
  
        /* proj mat */
        float vp_width = (float)ctx->device_settings.device_viewport[0];
        float vp_height = (float)ctx->device_settings.device_viewport[1];
        float ratio = vp_width / vp_height;
        float fov = cam.field_of_view;
        float near_p = cam.near_plane;
        float far_p = cam.far_plane;
  
        roa_mat4 proj;
        roa_mat4_projection(&proj, fov, near_p, far_p, ratio);
        roa_mat4_export(&proj, rp->camera_projection);

        /* view proj */
        roa_mat4 view_proj;
        roa_mat4_multiply(&view_proj, &view, &proj);
        roa_mat4_export(&view_proj, rp->camera_view_projection);
  
        /* cam pos */
        unsigned pos_size = sizeof(rp->camera.position);
        memcpy(rp->camera.position, cam.position, pos_size);

        /* needs to hold a lock */
        struct roa_renderer_task task;
        task.func = task_draw_calls;
        task.arg1 = arg1;
        task.arg2 = arg2;
        task.arg3 = 0;

        roa_array_push(ctx->tasks.back_tasks, task);
}
