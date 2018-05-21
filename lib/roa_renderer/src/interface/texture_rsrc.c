#include <roa_renderer/roa_renderer.h>
#include <common/alloc.h>
#include <roa_lib/hash.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>
#include <string.h>
#include <ctx/ctx.h>


/* ------------------------------------------------- [ Texture Resources ] -- */


unsigned
roa_renderer_texture_resource_count(
  roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  unsigned count = 0;

  {
    roa_spin_lock_aquire(&ctx->resource_desc.lock);
    count = roa_array_size(ctx->resource_desc.texture_ids);
    roa_spin_lock_release(&ctx->resource_desc.lock);
  }

  return count;
}


uint64_t
roa_renderer_texture_resource_add(
	roa_renderer_ctx_t ctx,
	const struct roa_renderer_texture_resource *rsrc)
{
  /* param */
  ROA_ASSERT(ctx);
  ROA_ASSERT(rsrc);
  ROA_ASSERT(rsrc->name)

  if (!ctx || !rsrc || !rsrc->name)
  {
    return 0;
  }
  
  uint64_t hash_name = roa_hash(rsrc->name);

  if (roa_renderer_texture_resource_exists(ctx, rsrc->name))
  {
    return hash_name;
  }
  else
  {
    struct roa_renderer_texture_resource copy;
    copy.name       = rdr_alloc_copy((void*)rsrc->name, strlen(rsrc->name) + 1);
    copy.data       = rdr_alloc_copy(rsrc->data, rsrc->width * rsrc->height * rsrc->components);
    copy.width      = rsrc->width;
    copy.height     = rsrc->height;
    copy.components = rsrc->components;

    roa_array_push(ctx->resource_desc.texture_rsrc_data, copy);
    roa_array_push(ctx->resource_desc.texture_ids, hash_name);

    roa_array_push(ctx->resource_desc.texture_rsrc_pending_data, copy);
    roa_array_push(ctx->resource_desc.texture_pending_ids, hash_name);
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_renderer_texture_resource_exists(
  roa_renderer_ctx_t ctx,
  const char *name)
{
  /* param */
  ROA_ASSERT(ctx);
  ROA_ASSERT(name);

  if (!ctx || !name)
  {
    return ROA_FALSE;
  }

  uint64_t hash_name = roa_hash(name);

  int count = roa_array_size(ctx->resource_desc.texture_ids);
  int i;
  uint64_t *texture_ids = ctx->resource_desc.texture_ids;

  for (i = 0; i < count; ++i)
  {
    if (texture_ids[i] == hash_name)
    {
      return ROA_TRUE;
    }
  }

  return ROA_FALSE;
}