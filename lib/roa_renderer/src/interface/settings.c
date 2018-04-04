#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>


/* ---------------------------------------------------------- [ Settings ] -- */


void
roa_renderer_set_device_resolution(
  roa_renderer_ctx_t ctx,
  unsigned width,
  unsigned height)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(width);
  ROA_ASSERT(height);

  /* set */
  ctx->settings.device_viewport[0] = width;
  ctx->settings.device_viewport[1] = height;
}