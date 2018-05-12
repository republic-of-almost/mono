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

}


ROA_BOOL
roa_renderer_mesh_renderable_get(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_mesh_renderable *out_renderable,
  uint32_t renderable_id)
{
 
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
