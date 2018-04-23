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

  /* find key */
  unsigned cam_count = roa_array_size(ctx->camera_id);
  unsigned i;

  for (i = 0; i < cam_count; ++i)
  {
    if (ctx->camera_id[i] == camera_id)
    {
      ctx->camera[i] = *camera;

      return ROA_TRUE;
    }
  }

  roa_array_push(ctx->camera_id, camera_id);
  roa_array_push(ctx->camera, *camera);

  return ROA_TRUE;
}


ROA_BOOL
roa_renderer_camera_get(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_camera *out_camera,
  uint32_t camera_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(out_camera);
  ROA_ASSERT(camera_id);

  /* find key */
  unsigned cam_count = roa_array_size(ctx->camera_id);
  unsigned i;

  for (i = 0; i < cam_count; ++i)
  {
    if (ctx->camera_id[i] == camera_id)
    {
      *out_camera = ctx->camera[i];

      return ROA_TRUE;
    }
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_renderer_camera_clear(
  roa_renderer_ctx_t ctx,
  uint32_t camera_id)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(camera_id);


	return ROA_TRUE;
}
