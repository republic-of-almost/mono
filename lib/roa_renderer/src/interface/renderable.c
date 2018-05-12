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

        ctx->renderer_desc.mesh_rdr_descs[i] = *renderable;

        roa_spin_lock_release(&ctx->renderer_desc.lock);
        return result;
      }
    }
  }

  /* insert new one */
  result = ROA_TRUE;

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
roa_renderer_renderable_clear(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
 
}



ROA_BOOL
roa_renderer_mesh_renderable_clear(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
 
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

}
