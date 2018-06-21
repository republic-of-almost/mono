#include <task/task.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <roa_math/math.h>


void
task_camera_mats(void *arg1, void *arg2, void *arg3) {
        ROA_ASSERT(arg1);
        ROA_ASSERT(arg2);
        ROA_UNUSED(arg3);

        printf("TASK : CAM MATS\n");
        
        struct roa_renderer_ctx *ctx = (struct roa_renderer_ctx*)arg1;
        struct renderpass *rp = (struct renderpass*)arg2;

        struct roa_renderer_camera cam = rp->camera;

        /* calculate camera mats */
        roa_mat4 view;
        roa_float3 pos = roa_float3_import(cam.position);
        roa_float3 up = roa_float3_import(cam.up);
        roa_float3 at = roa_float3_import(cam.lookat);

        roa_mat4_lookat(&view, pos, at, up);
        unsigned view_size = sizeof(rp->camera_view);
        memcpy(rp->camera_view, view.data, view_size);

        roa_mat4 proj;
        float ratio = (float)ctx->device_settings.device_viewport[0] / (float)ctx->device_settings.device_viewport[1];

        roa_mat4_projection(&proj, cam.field_of_view, cam.near_plane, cam.far_plane, ratio);
        unsigned proj_size = sizeof(rp->camera_projection);
        memcpy(rp->camera_projection, proj.data, proj_size);

        roa_mat4 view_proj;
        roa_mat4_multiply(&view_proj, &view, &proj);

        unsigned view_proj_size = sizeof(rp->camera_view_projection);
        memcpy(rp->camera_view_projection, &view_proj, view_proj_size);

        unsigned pos_size = sizeof(rp->camera.position);
        memcpy(rp->camera.position, cam.position, pos_size);

        rp->draw_calls = 0;
        unsigned dc_count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
        roa_array_create_with_capacity(rp->draw_calls, dc_count);


        /* needs to hold a lock */
        struct roa_renderer_task task;
        task.func = task_draw_calls;
        task.arg1 = arg1;
        task.arg2 = arg2;
        task.arg3 = 0;

        roa_array_push(ctx->tasks.back_tasks, task);
}
