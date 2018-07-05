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
                
                struct decal_transform *decal = &rp->decals[i];

                decal->time += 0.1;
                decal->time = roa_float_clamp(decal->time, 0.f, 1.f);

                roa_transform final_trans;
                final_trans.position = roa_float3_import(decal->final_pos);
                final_trans.scale = roa_float3_import(decal->final_scale);
                final_trans.rotation = roa_quaternion_import(decal->rotation);

                roa_transform zero_trans;
                zero_trans.position = roa_float3_import(decal->final_pos);
                zero_trans.scale = roa_float3_import(decal->final_scale);
                zero_trans.rotation = roa_quaternion_import(decal->rotation);


        }
}
