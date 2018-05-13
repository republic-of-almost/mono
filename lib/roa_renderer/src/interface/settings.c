#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/spin_lock.h>


/* ---------------------------------------------------------- [ Settings ] -- */


void
roa_renderer_set_device_viewport(
  roa_renderer_ctx_t ctx,
  const struct roa_renderer_device_viewport *vp)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(vp);

  roa_spin_lock_aquire(&ctx->device_settings.lock);

  ctx->device_settings.device_viewport[0] = vp->width;
  ctx->device_settings.device_viewport[1] = vp->height;

  roa_spin_lock_release(&ctx->device_settings.lock);
}


void
roa_renderer_get_device_viewport(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_device_viewport *out_vp)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(out_vp);

  roa_spin_lock_aquire(&ctx->device_settings.lock);

  out_vp->width = ctx->device_settings.device_viewport[0];
  out_vp->height = ctx->device_settings.device_viewport[1];

  roa_spin_lock_release(&ctx->device_settings.lock);
}