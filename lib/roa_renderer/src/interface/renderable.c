#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>


/* -------------------------------------------------------- [ Renderable ] -- */


ROA_BOOL
roa_renderer_mesh_renderable_create(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable_id);

  if (!ctx || !renderable_id)
  {
    return ROA_FALSE;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* check to see if exists */
  {
    uint32_t *ids = ctx->renderer_desc.mesh_rdr_ids;
    int count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == renderable_id)
      {
        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return ROA_FALSE;
      }
    }
  }

  /* need to copy array data */

  struct renderer_mesh_renderable renderable;
  ROA_MEM_ZERO(renderable);

  roa_array_push(ctx->renderer_desc.mesh_rdr_ids, renderable_id);
  roa_array_push(ctx->renderer_desc.mesh_rdr_descs, renderable);

  roa_spin_lock_release(&ctx->renderer_desc.lock);

  return ROA_TRUE;
}


ROA_BOOL
roa_renderer_mesh_renderable_destroy(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
    /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable_id);

  if (!ctx || !renderable_id)
  {
    return ROA_FALSE;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* check to see if exists */
  {
    uint32_t *ids = ctx->renderer_desc.mesh_rdr_ids;
    int count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == renderable_id)
      {
        roa_array_erase(ctx->renderer_desc.mesh_rdr_descs, i);
        roa_array_erase(ctx->renderer_desc.mesh_rdr_ids, i);

        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return ROA_TRUE;
      }
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  return ROA_FALSE;
}


ROA_BOOL
roa_renderer_mesh_renderable_transform_set(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id,
  float *transform)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable_id);
  ROA_ASSERT(transform);

  if (!ctx || !renderable_id || !transform)
  {
    return ROA_FALSE;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* check to see if exists */
  {
    uint32_t *ids = ctx->renderer_desc.mesh_rdr_ids;
    int count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == renderable_id)
      {
        int size = sizeof(ctx->renderer_desc.mesh_rdr_descs[i].world_transform);
        void *dst = &ctx->renderer_desc.mesh_rdr_descs[i].world_transform;

        memset(dst, transform, size);

        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return ROA_TRUE;
      }
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  return ROA_FALSE;
}


ROA_BOOL
roa_renderer_mesh_renderable_transform_get(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id,
  float *out_transform)
{
    /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable_id);
  ROA_ASSERT(out_transform);

  if (!ctx || !renderable_id || !out_transform)
  {
    return ROA_FALSE;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* check to see if exists */
  {
    uint32_t *ids = ctx->renderer_desc.mesh_rdr_ids;
    int count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == renderable_id)
      {
        int size = sizeof(ctx->renderer_desc.mesh_rdr_descs[i].world_transform);
        void *src = &ctx->renderer_desc.mesh_rdr_descs[i].world_transform;

        memset(out_transform, src, size);
        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return ROA_TRUE;
      }
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  return ROA_FALSE;
}


unsigned
roa_renderer_mesh_renderable_count(
  roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  unsigned count = 0;

  {
    roa_spin_lock_aquire(&ctx->renderer_desc.lock);
    count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    roa_spin_lock_release(&ctx->renderer_desc.lock);
  }

  return count;
}


ROA_BOOL
roa_renderer_mesh_renderable_exists(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable_id);

  if (!ctx || !renderable_id)
  {
    return ROA_FALSE;
  }

  ROA_BOOL result = ROA_FALSE;
  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* search */
  {
    uint32_t *ids = ctx->renderer_desc.mesh_rdr_ids;
    int count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == renderable_id)
      {
        result = ROA_TRUE;
        break;
      }
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  return result;
}
