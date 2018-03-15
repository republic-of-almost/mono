#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>


/* -------------------------------------------------------- [ Renderable ] -- */


ROA_BOOL
roa_renderer_renderable_set(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_renderable *renderable,
  uint32_t renderable_id)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(renderable);
	ROA_ASSERT(renderable_id);

	/* push new renderable in */
	roa_array_push(ctx->renderable_id, renderable_id);
	roa_array_push(ctx->renderable, *renderable);

  return ROA_TRUE;
}


ROA_BOOL
roa_renderer_renderable_clear(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id)
{
  return ROA_FALSE;
}
