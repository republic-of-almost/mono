#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/log.h>
#include <roa_lib/atomic.h>
#include <roa_lib/spin_lock.h>
#include <graphics_api/platform.h>
#include <common/alloc.h>


/* ---------------------------------------------------------- [ Lifetime ] -- */


ROA_BOOL
roa_renderer_ctx_create(
	roa_renderer_ctx_t *ctx,
  struct roa_renderer_ctx_desc *desc)
{
  /* param check */
  ROA_ASSERT(ctx);

  /* create new ctx */
  {
    struct roa_renderer_ctx *new_ctx = rdr_alloc(sizeof(*new_ctx));
    ROA_ASSERT(new_ctx);

    /* lock everything down - it shouldn't matter but just incase */
    roa_spin_lock_init(&new_ctx->device_settings.lock);
    roa_spin_lock_aquire(&new_ctx->device_settings.lock);

    roa_spin_lock_init(&new_ctx->renderer_desc.lock);
    roa_spin_lock_aquire(&new_ctx->renderer_desc.lock);

    roa_spin_lock_init(&new_ctx->device_settings.lock);
    roa_spin_lock_aquire(&new_ctx->device_settings.lock);

    roa_spin_lock_init(&new_ctx->renderpass.lock);
    roa_spin_lock_aquire(&new_ctx->renderpass.lock);

    roa_spin_lock_init(&new_ctx->resource_desc.lock);
    roa_spin_lock_aquire(&new_ctx->resource_desc.lock);
    

    /* device settings */
    {
      new_ctx->device_settings.device_viewport[0] = 800;
      new_ctx->device_settings.device_viewport[1] = 480;

      roa_spin_lock_release(&new_ctx->device_settings.lock);
    }


    /* renderer_desc */
    {
      roa_array_create_with_capacity(new_ctx->renderer_desc.camera_ids, 5);
      roa_array_create_with_capacity(new_ctx->renderer_desc.camera_descs, 5);

      roa_array_create_with_capacity(new_ctx->renderer_desc.mesh_rdr_ids, 256);
      roa_array_create_with_capacity(new_ctx->renderer_desc.mesh_rdr_descs, 256);

      roa_array_create_with_capacity(new_ctx->renderer_desc.light_ids, 64);
      roa_array_create_with_capacity(new_ctx->renderer_desc.light_descs, 64);

      roa_spin_lock_release(&new_ctx->renderer_desc.lock);
    }

    /* resource_desc */
    {
      roa_array_create_with_capacity(new_ctx->resource_desc.material_ids, 32);
      roa_array_create_with_capacity(new_ctx->resource_desc.mat_descs, 32);

      roa_array_create_with_capacity(new_ctx->resource_desc.mesh_ids, 128);
      roa_array_create_with_capacity(new_ctx->resource_desc.mesh_rsrc_data, 32);

      roa_spin_lock_release(&new_ctx->resource_desc.lock);
    }

    /* platform */
    {
      platform_setup(new_ctx);
    }

    *ctx = new_ctx;
  }
}


void
roa_renderer_ctx_execute(
	roa_renderer_ctx_t ctx)
{
  
}


void
roa_renderer_ctx_destroy(
	roa_renderer_ctx_t *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  {
    struct roa_renderer_ctx *kill_ctx = *ctx;
    ROA_ASSERT(kill_ctx);

    /* renderer_desc */
    {
      roa_array_destroy(kill_ctx->renderer_desc.camera_ids);
      roa_array_destroy(kill_ctx->renderer_desc.camera_descs);

      roa_array_destroy(kill_ctx->renderer_desc.mesh_rdr_ids);
      roa_array_destroy(kill_ctx->renderer_desc.mesh_rdr_descs);
      
      roa_array_destroy(kill_ctx->renderer_desc.light_ids);
      roa_array_destroy(kill_ctx->renderer_desc.light_descs);
    }

    /* resource desc */
    {
      roa_array_destroy(kill_ctx->resource_desc.material_ids);
      roa_array_destroy(kill_ctx->resource_desc.mat_descs);

      roa_array_destroy(kill_ctx->resource_desc.mesh_ids);
      roa_array_destroy(kill_ctx->resource_desc.mesh_rsrc_data);
    }

    rdr_free(kill_ctx);
    *ctx = ROA_NULL;
  }
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
