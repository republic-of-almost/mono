#include <roa_renderer/roa_renderer.h>
#include <roa_lib/hash.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>
#include <string.h>
#include <ctx/ctx.h>


/* ---------------------------------------------------- [ Mesh Resources ] -- */


unsigned
roa_renderer_mesh_resource_count(
  roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  unsigned count = 0;

  {
    roa_spin_lock_aquire(&ctx->resource_desc.lock);
    count = roa_array_size(ctx->resource_desc.mesh_ids);
    roa_spin_lock_release(&ctx->resource_desc.lock);
  }

  return count;
}


uint64_t
roa_renderer_mesh_resource_add(
	roa_renderer_ctx_t ctx,
	const struct roa_renderer_mesh_resource *rsrc)
{
  return 0;
}


int
roa_renderer_mesh_resource_remove(
  roa_renderer_ctx_t ctx,
  const char *name)
{
  return 0;
}
