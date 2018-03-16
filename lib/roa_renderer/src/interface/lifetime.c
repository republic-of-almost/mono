#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/log.h>


/* ---------------------------------------------------------- [ Lifetime ] -- */


ROA_BOOL
roa_renderer_ctx_create(
	roa_renderer_ctx_t *ctx)
{
	/* param check */
	ROA_ASSERT(ctx);

	struct roa_renderer_ctx *new_ctx = roa_zalloc(sizeof(*new_ctx));
	ROA_ASSERT(new_ctx);

  unsigned count = 1 << 10;

	roa_array_create_with_capacity(new_ctx->camera_id, count);
	roa_array_create_with_capacity(new_ctx->camera, count);
	roa_array_create_with_capacity(new_ctx->renderable_id, count);
	roa_array_create_with_capacity(new_ctx->renderable, count);

	*ctx = new_ctx;

	return new_ctx ? ROA_TRUE : ROA_FALSE;
}


void
roa_renderer_ctx_execute(
	roa_renderer_ctx_t ctx)
{
	/* param check */
	ROA_ASSERT(ctx);
	
	ROA_LOG_INFO("Camera count %d", roa_array_size(ctx->camera));
	ROA_LOG_INFO("Renderable count %d", roa_array_size(ctx->renderable));
}


void
roa_renderer_ctx_destroy(
	roa_renderer_ctx_t *ctx)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(*ctx);
	
	struct roa_renderer_ctx *kill_ctx = *ctx;

	roa_array_destroy(kill_ctx->renderable);
	roa_array_destroy(kill_ctx->renderable_id);
	roa_array_destroy(kill_ctx->camera);
	roa_array_destroy(kill_ctx->camera_id);

	roa_free(kill_ctx);

	*ctx = ROA_NULL;
}


void
roa_renderer_ctx_lock(
  roa_renderer_ctx_t ctx)
{
}


void
roa_renderer_ctx_unlock(
  roa_renderer_ctx_t ctx)
{
}

