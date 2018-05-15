#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>


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

  if (!ctx || !camera || !camera_id)
  {
    result = ROA_FALSE;
    return result;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* search and replace */
  {
    uint32_t *ids = ctx->renderer_desc.camera_ids;
    int count = roa_array_size(ctx->renderer_desc.camera_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == camera_id)
      {
        result = ROA_TRUE;

        ctx->renderer_desc.camera_descs[i] = *camera;
        
        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return result;
      }
    }
  }

  /* insert new one */
  result = ROA_TRUE;

  roa_array_push(ctx->renderer_desc.camera_ids, camera_id);
  roa_array_push(ctx->renderer_desc.camera_descs, *camera);

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

  if (!ctx || !out_camera || !camera_id)
  {
    result = ROA_FALSE;
    return result;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* search */
  {
    uint32_t *ids = ctx->renderer_desc.camera_ids;
    int count = roa_array_size(ctx->renderer_desc.camera_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == camera_id)
      {
        result = ROA_TRUE;

        *out_camera = ctx->renderer_desc.camera_descs[i];
        break;
      }
    }
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

  if (!ctx || !camera_id)
  {
    result = ROA_FALSE;
    return result;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* search */
  {
    uint32_t *ids = ctx->renderer_desc.camera_ids;
    int count = roa_array_size(ctx->renderer_desc.camera_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == camera_id)
      {
        result = ROA_TRUE;
        
        roa_array_erase(ctx->renderer_desc.camera_ids, i);
        roa_array_erase(ctx->renderer_desc.camera_descs, i);
        
        break;
      }
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

  if (!ctx)
  {
    return 0;
  }

  unsigned count = 0;

  {
    roa_spin_lock_aquire(&ctx->renderer_desc.lock);
    count = roa_array_size(ctx->renderer_desc.camera_ids);
    roa_spin_lock_release(&ctx->renderer_desc.lock);
  }

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

  if (!ctx || !camera_id)
  {
    return ROA_FALSE;
  }

  ROA_BOOL result = ROA_FALSE;
  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* search */
  {
    uint32_t *ids = ctx->renderer_desc.camera_ids;
    int count = roa_array_size(ctx->renderer_desc.camera_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == camera_id)
      {
        result = ROA_TRUE;
        break;
      }
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  return result;
}
