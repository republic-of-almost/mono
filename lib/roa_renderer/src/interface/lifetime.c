#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/log.h>
#include <roa_lib/atomic.h>
#include <graphics_api/platform.h>

/* ---------------------------------------------------------- [ Lifetime ] -- */


ROA_BOOL
roa_renderer_ctx_create(
	roa_renderer_ctx_t *ctx,
  struct roa_renderer_ctx_desc *desc)
{
	/* param check */
	ROA_ASSERT(ctx);

  struct renderer_alloc alloc;

  if (desc)
  {
    alloc.alloc = desc->alloc;
    alloc.free = desc->free;

    alloc.frame_alloc = desc->task_alloc;
    alloc.frame_free = desc->task_free;
  }
  else
  {
    alloc.alloc = roa_zalloc;
    alloc.free = roa_free;

    alloc.frame_alloc = roa_zalloc;
    alloc.frame_free = roa_free;
  }

	struct roa_renderer_ctx *new_ctx = alloc.alloc(sizeof(*new_ctx));
	ROA_ASSERT(new_ctx);

  new_ctx->mem = alloc;

  unsigned count = 1 << 10;

  roa_array_create_with_capacity(new_ctx->mesh_renderpasses, count);
	roa_array_create_with_capacity(new_ctx->camera_id, count);
	roa_array_create_with_capacity(new_ctx->camera, count);
	roa_array_create_with_capacity(new_ctx->renderable_id, count);
	roa_array_create_with_capacity(new_ctx->renderable, count);

	/* mesh rsrc */
	roa_array_create_with_capacity(new_ctx->mesh_rsrc.rsrc, count);
	roa_array_create_with_capacity(new_ctx->mesh_rsrc.create_ids, count);
	roa_array_create_with_capacity(new_ctx->mesh_rsrc.ids, count);
	roa_spin_lock_init(&new_ctx->mesh_rsrc.lock);

  /* settings */
  new_ctx->settings.device_viewport[0] = 800;
  new_ctx->settings.device_viewport[1] = 480;

  *ctx = new_ctx;

	platform_setup(ctx);

	return new_ctx ? ROA_TRUE : ROA_FALSE;
}


void
roa_renderer_ctx_execute(
	roa_renderer_ctx_t ctx)
{
	/* param check */
	ROA_ASSERT(ctx);

	platform_update(ctx);
	platform_render(ctx);
}


void
roa_renderer_ctx_destroy(
	roa_renderer_ctx_t *ctx)
{
	/* param check */
	ROA_ASSERT(ctx);
	ROA_ASSERT(*ctx);

	platform_destroy(ctx);

	struct roa_renderer_ctx *kill_ctx = *ctx;

  roa_array_destroy(kill_ctx->mesh_renderpasses);
	roa_array_destroy(kill_ctx->renderable);
	roa_array_destroy(kill_ctx->renderable_id);
	roa_array_destroy(kill_ctx->camera);
	roa_array_destroy(kill_ctx->camera_id);
  
  roa_renderer_free free_fn = kill_ctx->mem.free;

  free_fn(kill_ctx);

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
