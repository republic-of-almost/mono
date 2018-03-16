#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>


ROA_BOOL
roa_renderer_camera_set(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_camera *camera,
  uint32_t camera_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(camera);
  ROA_ASSERT(camera_id);

	/* push camera */
	roa_array_push(ctx->camera_id, camera_id);
	roa_array_push(ctx->camera, *camera);

  return ROA_TRUE;
}


ROA_BOOL
roa_renderer_camera_clear(
  roa_renderer_ctx_t ctx,
  uint32_t camera_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(camera_id);

}
