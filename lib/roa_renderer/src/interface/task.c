#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <task/task.h>
#include <roa_math/math.h>
#include <stdio.h>
#include <roa_lib/spin_lock.h>


unsigned
roa_renderer_task_pump(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_task **tasks)
{
  roa_spin_lock_aquire(&ctx->renderer_desc.lock);
  roa_spin_lock_aquire(&ctx->renderpass.lock);
  roa_spin_lock_aquire(&ctx->device_settings.lock);
  
  unsigned cam_count = roa_array_size(ctx->renderer_desc.camera_ids);
  roa_array_resize(ctx->renderpass.rps, cam_count);

  int i = 0;
  for (i = 0; i < cam_count; ++i)
  {
    struct roa_renderer_camera cam = ctx->renderer_desc.camera_descs[i];
    struct renderpass *rp = &ctx->renderpass.rps[i];

    /* calculate camera mats */
    roa_mat4 view;
    roa_float3 pos = roa_float3_import(cam.position);
    roa_float3 up  = roa_float3_import(cam.up);
    roa_float3 at  = roa_float3_import(cam.lookat);

    roa_mat4_lookat(&view, pos, at, up);
    unsigned view_size = sizeof(rp->camera.view);
    memcpy(rp->camera.view, view.data, view_size);

    roa_mat4 proj;
    float ratio = (float)ctx->device_settings.device_viewport[0] / (float)ctx->device_settings.device_viewport[1];
    
    roa_mat4_projection(&proj, cam.field_of_view, cam.near_plane, cam.far_plane, ratio);
    unsigned proj_size = sizeof(rp->camera.projection);
    memcpy(rp->camera.projection, proj.data, proj_size);

    roa_mat4 view_proj;
    roa_mat4_multiply(&view_proj, &view, &proj);

    unsigned view_proj_size = sizeof(rp->camera.view_projection);
    memcpy(rp->camera.view_projection, &view_proj, view_proj_size);

    unsigned pos_size = sizeof(rp->camera.position);
    memcpy(rp->camera.position, cam.position, pos_size);

    rp->draw_calls = 0;
    unsigned dc_count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    roa_array_create_with_capacity(rp->draw_calls, dc_count);

    rp->decals = 0;
    roa_array_create_with_capacity(rp->decals, dc_count * 4);

    unsigned j;

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

    /* decals */
    for (j = 0; j < dc_count; ++j) {
            struct renderer_mesh_renderable rdr = ctx->renderer_desc.mesh_rdr_descs[j];

            roa_transform parent_trans;
            parent_trans.position = roa_float3_import(rdr.position);
            parent_trans.rotation = roa_quaternion_import(rdr.rotation);
            parent_trans.scale = roa_float3_import(rdr.scale);

            roa_float3 distance = roa_float3_subtract(parent_trans.position, roa_float3_import(cam.position));
            float lod_distance = roa_float_abs(roa_float3_length(distance));

            int mesh_id = rdr.mesh_id;

            unsigned mesh_count = roa_array_size(ctx->resource_desc.mesh_ids);
            uint64_t *mesh_ids = ctx->resource_desc.mesh_ids;

            int k;
            for (k = 0; k < mesh_count; ++k)
            {
                    if(k == 0) {
                            struct roa_renderer_mesh_resource *rsrc = &ctx->resource_desc.mesh_rsrc_data[k];

                            struct roa_renderer_decal *lods[10] = {
                                    rsrc->decals_lod0,
                                    rsrc->decals_lod1,
                                    rsrc->decals_lod2,
                                    rsrc->decals_lod3,
                                    rsrc->decals_lod4,
                                    rsrc->decals_lod5,
                                    rsrc->decals_lod6,
                                    rsrc->decals_lod7,
                                    rsrc->decals_lod8,
                                    rsrc->decals_lod9,
                            };

                            int *lod_count[10] = {
                                    rsrc->decals_lod0_count,
                                    rsrc->decals_lod1_count,
                                    rsrc->decals_lod2_count,
                                    rsrc->decals_lod3_count,
                                    rsrc->decals_lod4_count,
                                    rsrc->decals_lod5_count,
                                    rsrc->decals_lod6_count,
                                    rsrc->decals_lod7_count,
                                    rsrc->decals_lod8_count,
                                    rsrc->decals_lod9_count,
                            };

                            
                            float dist = lod_distance > 0.f ? (lod_distance + 0.5f) : (lod_distance - 0.5f);
                            int lod_max = 10 - (int)dist / 10;
                            lod_max = lod_max > 10 ? 10 : lod_max < 0 ? 0 : lod_max;

                            int l;
                            int m;

                            for(m = 0; m < lod_max; ++m)
                            {

                                    int decal_count = lod_count[m];

                                    for(l = 0; l < decal_count; ++l) {
                                            struct roa_renderer_decal *decal = &lods[m][l];

                                            roa_transform decal_trans;
                                            decal_trans.position  = roa_float3_import(decal->position);
                                            decal_trans.rotation  = roa_quaternion_import(decal->rotation);
                                            decal_trans.scale     = roa_float3_import(decal->scale);

                                            roa_transform inherited;
                                            roa_transform_inherited(&inherited, &parent_trans, &decal_trans);

                                            struct decal_transform decal_t;
                                            roa_transform_to_mat4(&inherited, decal_t.world_mat);

                                            roa_array_push(rp->decals, decal_t);
                                    }
                            }
          
                            break;
                    }
            }
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  roa_spin_lock_release(&ctx->renderpass.lock);
  roa_spin_lock_release(&ctx->device_settings.lock);

  return 0;
}


void
roa_renderer_task_run(
	struct roa_renderer_task *task)
{

}
