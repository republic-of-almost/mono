#include <task/task.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <ctx/ctx.h>
#include <roa_math/math.h>
#include <string.h>
#include <stdio.h>


void
task_draw_calls(void *arg1, void *arg2, void *arg3) {
        
        ROA_ASSERT(arg1);
        ROA_ASSERT(arg2);
        ROA_UNUSED(arg3);

        printf("TASK : Draw calls\n");
        
        struct roa_renderer_ctx *ctx = (struct roa_renderer_ctx*)arg1;
        struct renderpass *rp = (struct renderpass*)arg2;

        rp->draw_calls = 0;
        unsigned dc_count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
        roa_array_create_with_capacity(rp->draw_calls, dc_count);

        struct roa_renderer_camera cam = rp->camera;

        int j;

        roa_mat4 view_proj;
        roa_mat4_import(&view_proj, rp->camera_view_projection);

        /* draw calls */
        for (j = 0; j < dc_count; ++j) {
                struct renderer_mesh_renderable rdr = ctx->renderer_desc.mesh_rdr_descs[j];

                struct renderpass_draw_call dc;
                roa_transform trans;
                trans.position = roa_float3_import(rdr.position);
                trans.rotation = roa_quaternion_import(rdr.rotation);
                trans.scale = roa_float3_import(rdr.scale);

                roa_mat4 world;
                roa_transform_to_mat4(&trans, &world);

                memcpy(dc.world, ROA_ARR_DATA(world.data), sizeof(dc.world));

                roa_mat4 wvp;
                roa_mat4_multiply(&wvp, &world, &view_proj);
                memcpy(dc.wvp, wvp.data, sizeof(dc.wvp));

                roa_array_push(rp->draw_calls, dc);
        }

        /* needs to hold a lock */
        struct roa_renderer_task task;
        task.func = task_decal_update;
        task.arg1 = arg1;
        task.arg2 = arg2;
        task.arg3 = 0;

        roa_array_push(ctx->tasks.back_tasks, task);
}
