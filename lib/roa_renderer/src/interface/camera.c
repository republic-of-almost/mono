#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>
#include <string.h>


/* ------------------------------------------------------------ [ Helper ] -- */


int
find_index(uint32_t *ids, int id_count, uint32_t id)
{
        int i;

        for (i = 0; i < id_count; ++i) {
                if (ids[i] == id) {
                        return i;
                }
        }

        return -1;
}



/* ------------------------------------------------------------ [ Camera ] -- */


ROA_BOOL
roa_renderer_camera_set(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_camera *camera,
  uint32_t camera_id)
{
        /* param check */
        ROA_ASSERT(ctx);
        ROA_ASSERT(camera);
        ROA_ASSERT(camera_id);

        ROA_BOOL result = ROA_FALSE;

        /* is valid? */
        if (!ctx || !camera || !camera_id) {
                result = ROA_FALSE;
                return result;
        }

        roa_spin_lock_aquire(&ctx->renderer_desc.lock);

        /* search and replace or insert */
        uint32_t *ids = ctx->renderer_desc.camera_ids;
        int count = roa_array_size(ctx->renderer_desc.camera_ids);
        int index = find_index(ids, count, camera_id);

        if(index >= 0) {
                result = ROA_TRUE;

                ctx->renderer_desc.camera_passes[index].camera = *camera;
        
                roa_spin_lock_release(&ctx->renderer_desc.lock);
        }
        else {
                result = ROA_TRUE;

                roa_array_push(ctx->renderer_desc.camera_ids, camera_id);

                struct renderpass rp;
                ROA_MEM_ZERO(rp);
                rp.camera = *camera;

                int max_decal_count = 256;

                roa_array_create_with_capacity(rp.decals, max_decal_count);
                roa_array_resize(rp.decals, max_decal_count);
                memset(rp.decals, 0, sizeof(rp.decals[0]) * max_decal_count);

                roa_array_create_with_capacity(rp.decal_ids, max_decal_count);
                roa_array_resize(rp.decal_ids, max_decal_count);
                memset(rp.decal_ids, 0, sizeof(rp.decal_ids[0]) * max_decal_count);

                roa_array_create_with_capacity(rp.decal_lru, max_decal_count);
                roa_array_resize(rp.decal_lru, max_decal_count);
                memset(rp.decal_lru, 0, sizeof(rp.decal_lru[0]) * max_decal_count);
                
                rp.decal_count = max_decal_count;

                roa_array_push(ctx->renderer_desc.camera_passes, rp);
        }
          
        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return result;
}


ROA_BOOL
roa_renderer_camera_get(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_camera *out_camera,
  uint32_t camera_id)
{
        /* param check */
        ROA_ASSERT(ctx);
        ROA_ASSERT(out_camera);
        ROA_ASSERT(camera_id);

        ROA_BOOL result = ROA_FALSE;
        
        /* is valid? */
        if (!ctx || !out_camera || !camera_id) {
                result = ROA_FALSE;
                return result;
        }

        roa_spin_lock_aquire(&ctx->renderer_desc.lock);

        /* search */
        uint32_t *ids = ctx->renderer_desc.camera_ids;
        int count = roa_array_size(ctx->renderer_desc.camera_ids);
        int index = find_index(ids, count, camera_id);

        if (index >= 0) {
                result = ROA_TRUE;

                *out_camera = ctx->renderer_desc.camera_passes[index].camera;
        }
  
        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return result;
}


ROA_BOOL
roa_renderer_camera_clear(
  roa_renderer_ctx_t ctx,
  uint32_t camera_id)
{
        /* param check */
        ROA_ASSERT(ctx);
        ROA_ASSERT(camera_id);

        ROA_BOOL result = ROA_FALSE;

        if (!ctx || !camera_id) {
                result = ROA_FALSE;
                return result;
        }

        roa_spin_lock_aquire(&ctx->renderer_desc.lock);

        /* search */        
        uint32_t *ids = ctx->renderer_desc.camera_ids;
        int count = roa_array_size(ctx->renderer_desc.camera_ids);
        int i;

        for (i = 0; i < count; ++i) {
                if (ids[i] == camera_id) {
                        result = ROA_TRUE;
        
                        roa_array_erase(ctx->renderer_desc.camera_ids, i);
                        roa_array_erase(ctx->renderer_desc.camera_passes, i);
        
                        break;
                }
        }
        
        roa_spin_lock_release(&ctx->renderer_desc.lock);

        return result;
}


unsigned
roa_renderer_camera_count(
  const roa_renderer_ctx_t ctx)
{
        /* param check */
        ROA_ASSERT(ctx);

        if (!ctx) {
                return 0;
        }

        unsigned count = 0;

        roa_spin_lock_aquire(&ctx->renderer_desc.lock);
        count = roa_array_size(ctx->renderer_desc.camera_ids);
        roa_spin_lock_release(&ctx->renderer_desc.lock);

        return count;
}


ROA_BOOL
roa_renderer_camera_exists(
  const roa_renderer_ctx_t ctx,
  uint32_t camera_id)
{
        /* param check */
        ROA_ASSERT(ctx);
        ROA_ASSERT(camera_id);

        /* is valid? */
        if (!ctx || !camera_id) {
                return ROA_FALSE;
        }

        ROA_BOOL result = ROA_FALSE;
        roa_spin_lock_aquire(&ctx->renderer_desc.lock);

        /* search */
        uint32_t *ids = ctx->renderer_desc.camera_ids;
        int count = roa_array_size(ctx->renderer_desc.camera_ids);
        int i;

        for (i = 0; i < count; ++i) {
                if (ids[i] == camera_id) {
                        result = ROA_TRUE;
                        break;
                }
        }

        roa_spin_lock_release(&ctx->renderer_desc.lock);

        return result;
}
