#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>


/* -------------------------------------------------------- [ Renderable ] -- */


ROA_BOOL
roa_renderer_mesh_renderable_set(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_mesh_renderable *renderable,
  uint32_t renderable_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable);
  ROA_ASSERT(renderable_id);

  ROA_BOOL result = ROA_FALSE;

  if (!ctx || !renderable || !renderable_id)
  {
    result = ROA_FALSE;
    return result;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* search and replace */
  {
    uint32_t *ids = ctx->renderer_desc.mesh_rdr_ids;
    int count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == renderable_id)
      {
        result = ROA_TRUE;

        struct roa_renderer_mesh_renderable copy;
        ROA_MEM_ZERO(copy);

        /* need free / malloc array data - this sucks */

        copy.mesh_id = renderable->mesh_id;
        copy.decals_lod0_count = renderable->decals_lod0_count;
        memcpy(copy.world_transform, renderable->world_transform, sizeof(copy.world_transform));
        memcpy(copy.decals_lod0, renderable->decals_lod0, sizeof(copy.decals_lod0[0]) * copy.decals_lod0_count);

        ctx->renderer_desc.mesh_rdr_descs[i] = *renderable;

        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return result;
      }
    }
  }

  /* insert new one */
  result = ROA_TRUE;

  /* need to copy array data */

  roa_array_push(ctx->renderer_desc.mesh_rdr_ids, renderable_id);
  roa_array_push(ctx->renderer_desc.mesh_rdr_descs, *renderable);

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  return result;
}


ROA_BOOL
roa_renderer_mesh_renderable_get(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_mesh_renderable *out_renderable,
  uint32_t renderable_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(out_renderable);
  ROA_ASSERT(renderable_id);

  ROA_BOOL result = ROA_FALSE;

  if (!ctx || !out_renderable || !renderable_id)
  {
    result = ROA_FALSE;
    return result;
  }

  roa_spin_lock_aquire(&ctx->renderer_desc.lock);

  /* search */
  {
    uint32_t *ids = ctx->renderer_desc.mesh_rdr_ids;
    int count = roa_array_size(ctx->renderer_desc.mesh_rdr_descs);
    int i;

    for (i = 0; i < count; ++i)
    {
      if (ids[i] == renderable_id)
      {
        result = ROA_TRUE;

        *out_renderable = ctx->renderer_desc.mesh_rdr_descs[i];
        break;
      }
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  return result;
}


ROA_BOOL
roa_renderer_mesh_renderable_clear(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(renderable_id);

  ROA_BOOL result = ROA_FALSE;

  if (!ctx || !renderable_id)
  {
    result = ROA_FALSE;
    return result;
  }

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

        roa_array_erase(ctx->renderer_desc.mesh_rdr_ids, i);
        roa_array_erase(ctx->renderer_desc.mesh_rdr_descs, i);

        break;
      }
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);

  return result;
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
