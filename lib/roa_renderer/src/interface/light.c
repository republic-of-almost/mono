#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <roa_lib/spin_lock.h>


ROA_BOOL
roa_renderer_light_set(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_light *light,
  uint32_t light_id)
{
        (void)ctx;
        (void)light;
        (void)light_id;

        return ROA_FALSE;
}


ROA_BOOL
roa_renderer_light_get(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_light *out_light,
  uint32_t light_id)
{
        (void)ctx;
        (void)out_light;
        (void)light_id;

        return ROA_FALSE;
}


ROA_BOOL
roa_renderer_light_clear(
  roa_renderer_ctx_t ctx,
  uint32_t light_id)
{
        (void)ctx;
        (void)light_id;

        return ROA_FALSE;
}


unsigned
roa_renderer_light_count(
  const roa_renderer_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  unsigned count = 0;

  {
    roa_spin_lock_aquire(&ctx->renderer_desc.lock);
    count = roa_array_size(ctx->renderer_desc.light_ids);
    roa_spin_lock_release(&ctx->renderer_desc.lock);
  }

  return count;
}


ROA_BOOL
roa_renderer_light_exists(
  const roa_renderer_ctx_t ctx,
  uint32_t light_id)
{
        (void)ctx;
        (void)light_id;

        return ROA_FALSE;
}
