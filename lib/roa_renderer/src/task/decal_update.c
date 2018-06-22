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
                struct renderer_mesh_renderable rdr = ctx->renderer_desc.mesh_rdr_descs[dc->rdr_id];

                roa_transform parent_trans;
                parent_trans.position = roa_float3_import(rdr.position);
                parent_trans.rotation = roa_quaternion_import(rdr.rotation);
                parent_trans.scale = roa_float3_import(rdr.scale);

                roa_float3 distance = roa_float3_subtract(
                        parent_trans.position,
                        roa_float3_import(cam.position));

                float lod_distance = roa_float_abs(roa_float3_length(distance));

                /* find mesh */
                int mesh_count = (int)roa_array_size(ctx->resource_desc.mesh_ids);

                struct roa_renderer_mesh_resource *rsrc = ROA_NULL;

                int k;
                for (k = 0; k < mesh_count; ++k) {
                        if (k == dc->rdr_id) {
                                rsrc = &ctx->resource_desc.mesh_rsrc_data[k];
                                break;
                        }
                }

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
                                int decal_count = lod_count[m];

                                for (l = 0; l < decal_count; ++l) {

                                        /* check if decal exists */
                                        uint32_t lod_id = ROA_PACK1616((uint16_t)m + 1, (uint16_t)l + 1);
                                        uint64_t decal_id = ROA_PACK3232(k + 1, lod_id);

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
                                        roa_transform_inherited(&inherited, &parent_trans, &decal_trans);

                                        struct decal_transform decal_t;
                                        roa_mat4 out_mat;
                                        roa_transform_to_mat4(&inherited, &out_mat);
                                        roa_mat4_export(&out_mat, &decal_t.world_mat[0]);

                                        memcpy(decal_t.color, decal->color, sizeof(decal_t.color));

                                        for (z = 0; z < decal_count; ++z) {
                                                if (rp->decal_ids[z] == 0) {
                                                        rp->decal_ids[z] = decal_id;
                                                        rp->decals[z] = decal_t;
                                                        break;
                                                }
                                        }

                                        //roa_array_push(rp->decals, decal_t);
                                }
                        }

                        break;
                }
        }

}
