#include <ctx/ctx.h>
#include <task/task.h>
#include <roa_math/math.h>
#include <roa_lib/assert.h>
#include <roa_lib/fundamental.h>

/* in the future this will deal with brush stroke animation */

void
task_decal_decay(
        void *arg1,
        void *arg2,
        void *arg3)
{
        /* param check */
        ROA_ASSERT(arg1);
        ROA_ASSERT(arg2);
        ROA_UNUSED(arg3);
  
        /* args */
//        struct roa_renderer_ctx *ctx = (struct roa_renderer_ctx*)arg1;
        struct renderpass *rp = (struct renderpass*)arg2;
        
        unsigned i;
        for(i = 0; i < rp->decal_count; ++i) {
        
                if(rp->decal_ids[i] == 0) {
                        continue;
                }
          
                struct decal_transform *decal = &rp->decals[i];

                decal->time += 0.002f;
                decal->time = roa_float_clamp(decal->time, 0.01f, 1.f);

                roa_float3 scale_mask = roa_float3_import(decal->scale);
                roa_float3 scale = roa_float3_scale(scale_mask, decal->time);
                scale = roa_float3_add(roa_float3_one(), scale);
                scale = roa_float3_subtract(scale, scale_mask);
          
                roa_float3 final_scale = roa_float3_import(decal->final_scale);
                scale = roa_float3_multiply(scale, final_scale);
          
                roa_transform final_trans;
                final_trans.position = roa_float3_import(decal->final_pos);
                final_trans.scale = scale;
                final_trans.rotation = roa_quaternion_import(decal->rotation);
          
                roa_mat4 world;
                roa_transform_to_mat4(&final_trans, &world);
          
                roa_mat4_export(&world, decal->world_mat);
        }
}
