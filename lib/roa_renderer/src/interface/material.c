#include <roa_renderer/roa_renderer.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/spin_lock.h>
#include <ctx/ctx.h>


unsigned
roa_renderer_material_resource_count(
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