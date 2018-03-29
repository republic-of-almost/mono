#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/log.h>


/* ---------------------------------------------------------- [ Lifetime ] -- */


ROA_BOOL
roa_renderer_ctx_create(
	roa_renderer_ctx_t *ctx,
  struct roa_renderer_desc *desc)
{
	/* param check */
	ROA_ASSERT(ctx);

	struct roa_renderer_ctx *new_ctx = roa_zalloc(sizeof(*new_ctx));
	ROA_ASSERT(new_ctx);

  unsigned count = 1 << 10;

  roa_array_create_with_capacity(new_ctx->mesh_camera_data, count);
  roa_array_create_with_capacity(new_ctx->mesh_rendering_data, count);
	roa_array_create_with_capacity(new_ctx->camera_id, count);
	roa_array_create_with_capacity(new_ctx->camera, count);
	roa_array_create_with_capacity(new_ctx->renderable_id, count);
	roa_array_create_with_capacity(new_ctx->renderable, count);
  roa_array_create_with_capacity(new_ctx->tasks, count);

	*ctx = new_ctx;

  volt_ctx_create(&new_ctx->volt_ctx);

	return new_ctx ? ROA_TRUE : ROA_FALSE;
}


void
roa_renderer_ctx_execute(
	roa_renderer_ctx_t ctx)
{
	/* param check */
	ROA_ASSERT(ctx);
	
	volt_ctx_execute(ctx->volt_ctx);
}


void
roa_renderer_ctx_destroy(
	roa_renderer_ctx_t *ctx)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(*ctx);
	
	struct roa_renderer_ctx *kill_ctx = *ctx;

  roa_array_destroy(kill_ctx->mesh_camera_data);
  roa_array_destroy(kill_ctx->mesh_rendering_data);
  roa_array_destroy(kill_ctx->tasks);
	roa_array_destroy(kill_ctx->renderable);
	roa_array_destroy(kill_ctx->renderable_id);
	roa_array_destroy(kill_ctx->camera);
	roa_array_destroy(kill_ctx->camera_id);

  volt_ctx_destroy(&kill_ctx->volt_ctx);

	roa_free(kill_ctx);

	*ctx = ROA_NULL;
}


void
roa_renderer_ctx_lock(
  roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);
}


void
roa_renderer_ctx_unlock(
  roa_renderer_ctx_t ctx)
{
  ROA_UNUSED(ctx);
}

