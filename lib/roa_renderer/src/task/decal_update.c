#include <task/task.h>
#include <ctx/ctx.h>
#include <roa_math/math.h>
#include <roa_lib/assert.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/array.h>
#include <string.h>


void
task_decal_update(
        void *arg1,
        void *arg2,
        void *arg3)
{
        /* param check */
        ROA_ASSERT(arg1);
        ROA_ASSERT(arg2);
        ROA_UNUSED(arg3);

        struct roa_renderer_ctx *ctx = (struct roa_renderer_ctx*)arg1;
        struct renderpass *rp = (struct renderpass*)arg2;

        int dc_count = (int)roa_array_size(rp->draw_calls);

        struct roa_renderer_camera cam = rp->camera;

        /* decals */
        int j;

        for (j = 0; j < dc_count; ++j) {
                struct renderpass_draw_call *dc = &rp->draw_calls[j];
//                struct renderer_mesh_renderable rdr = ctx->renderer_desc.mesh_rdr_descs[dc->rdr_id];

                roa_transform parent_trans;
                parent_trans.position = roa_float3_import(dc->position);
                parent_trans.rotation = roa_quaternion_import(dc->rotation);
                parent_trans.scale = roa_float3_import(dc->scale);

                roa_float3 distance = roa_float3_subtract(
                        parent_trans.position,
                        roa_float3_import(cam.position));

                float lod_distance = roa_float_abs(roa_float3_length(distance));

                /* find mesh */
                struct roa_renderer_mesh_resource *rsrc = &ctx->resource_desc.mesh_rsrc_data[dc->mesh_index];

                /* process decals */
                if (rsrc != ROA_NULL) {
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

                        int lod_count[10] = {
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
                        lod_max = lod_max > 10 ? 10 : lod_max < 1 ? 1 : lod_max;

                        int l;
                        int m;

                        for (m = 0; m < lod_max; ++m) {
                                int m_decal_count = lod_count[m];

                                for (l = 0; l < m_decal_count; ++l) {

                                        /* check if decal exists */
                                        uint32_t lod_id = ROA_PACK1616((uint16_t)m + 1, (uint16_t)l + 1);
                                        uint64_t decal_id = ROA_PACK3232(dc->mesh_index + 1, lod_id);

                                        int found = 0;

                                        int decal_count = (int)rp->decal_count;
                                        int z;
                                        for (z = 0; z < decal_count; ++z) {
                                                if (rp->decal_ids[z] == decal_id) {
                                                        found = 1;
                                                        break;
                                                }
                                        }

                                        if (found > 0) {
                                                continue;
                                        }

                                        struct roa_renderer_decal *decal = &lods[m][l];

                                        roa_transform decal_trans;

                                        decal_trans.position = roa_float3_import(decal->position);
                                        decal_trans.rotation = roa_quaternion_import(decal->rotation);
                                        decal_trans.scale = roa_float3_import(decal->scale);

                                        roa_transform inherited;
                                        roa_transform_inherited(&inherited, &decal_trans, &parent_trans);

                                        struct decal_transform decal_t;
                                        roa_mat4 out_mat;
                                        roa_transform_to_mat4(&inherited, &out_mat);
                                        roa_mat4_export(&out_mat, &decal_t.world_mat[0]);

                                        roa_float3_export(inherited.position, decal_t.final_pos);
                                        roa_float3_export(inherited.scale, decal_t.final_scale);
                                        roa_quaternion_export(inherited.rotation, decal_t.rotation);

                                        /* largest scale */
                                        if(inherited.scale.x >= roa_float_max(inherited.scale.y, inherited.scale.z)) {
                                                decal_t.scale[0] = 1;
                                                decal_t.scale[1] = 0;
                                                decal_t.scale[2] = 0;
                                        }
                                        else if(inherited.scale.y >= roa_float_max(inherited.scale.x, inherited.scale.z)) {
                                                decal_t.scale[0] = 0;
                                                decal_t.scale[1] = 1;
                                                decal_t.scale[2] = 0;
                                        }
                                        else if(inherited.scale.z >= roa_float_max(inherited.scale.x, inherited.scale.y)) {
                                                decal_t.scale[0] = 0;
                                                decal_t.scale[1] = 0;
                                                decal_t.scale[2] = 1;
                                        }
                                        
                                        /* set current positions to 0 */
                                        roa_float3 scale = roa_float3_import(decal_t.scale);
                                        scale = roa_float3_normalize(scale);

                                        roa_float3 rot = roa_quaternion_rotate_vector(inherited.rotation, scale);
                                        roa_float3_export(rot, decal_t.brush);

                                        decal_t.object_id = j;

                                        memcpy(decal_t.color, decal->color, sizeof(decal_t.color));
                                        
                                        for (z = 0; z < decal_count; ++z) {
                                                if (rp->decal_ids[z] == 0) {
                                                        rp->decal_ids[z] = decal_id;
                                                        rp->decals[z] = decal_t;
                                                        break;
                                                }
                                        }
                                }
                        }
                }
        }

}
