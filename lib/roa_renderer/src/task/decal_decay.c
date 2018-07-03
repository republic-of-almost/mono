#include <ctx/ctx.h>
#include <task/task.h>
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
                rp->decal_ids[i] = 0;
        }
}
