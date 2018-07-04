#include <task/task.h>
#include <ctx/ctx.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_math/math.h>
#include <string.h>


void
task_draw_calls(
        void *arg1,
        void *arg2,
        void *arg3)
{
        /* param check */
        ROA_ASSERT(arg1);
        ROA_ASSERT(arg2);
        ROA_UNUSED(arg3);
  
        /* get args */
        struct roa_renderer_ctx *ctx = (struct roa_renderer_ctx*)arg1;
        struct renderpass *rp = (struct renderpass*)arg2;

        if(rp->draw_calls) {
                roa_array_destroy(rp->draw_calls);
        }

        rp->draw_calls = ROA_NULL;
        unsigned dc_count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
        roa_array_create_with_capacity(rp->draw_calls, dc_count);

        unsigned j;

        roa_mat4 view_proj;
        roa_mat4_import(&view_proj, rp->camera_view_projection);

        /* draw calls */
        for (j = 0; j < dc_count; ++j) {
                struct renderer_mesh_renderable rdr =
                        ctx->renderer_desc.mesh_rdr_descs[j];

                struct renderpass_draw_call dc;

                dc.object_id = (float)j; /*ctx->renderer_desc.mesh_rdr_ids[j];*/
          
                uint64_t mesh_id = rdr.mesh_id;
          
                /* all this is broken */
          
                unsigned k;
                unsigned mesh_count = roa_array_size(ctx->resource_desc.mesh_ids);
          
                for(k = 0; k < mesh_count; ++k) {
                        uint64_t this_id = ctx->resource_desc.mesh_ids[k];
                        if(this_id == mesh_id) {
                                dc.mesh_index = k;
                                break;
                        }
                }
          
                roa_transform trans;
                trans.position = roa_float3_import(rdr.position);
                trans.rotation = roa_quaternion_import(rdr.rotation);
                trans.scale = roa_float3_import(rdr.scale);

                roa_mat4 world;
                roa_transform_to_mat4(&trans, &world);
                roa_transform_export_mat4(&trans, dc.world);

                roa_mat4 wvp;
                roa_mat4_multiply(&wvp, &world, &view_proj);
                roa_mat4_export(&wvp, dc.wvp);

                memcpy(dc.position, &trans.position, sizeof(dc.position));
                memcpy(dc.rotation, &trans.rotation, sizeof(dc.rotation));
                memcpy(dc.scale, &trans.scale, sizeof(dc.scale));

                roa_array_push(rp->draw_calls, dc);
        }

        /* push decal tasks */
        /* needs to hold a lock */
        struct roa_renderer_task task;
        task.func = task_decal_update;
        task.arg1 = arg1;
        task.arg2 = arg2;
        task.arg3 = 0;

        roa_array_push(ctx->tasks.back_tasks, task);
}
