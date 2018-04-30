#include <task/task.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <roa_lib/assert.h>
#include <roa_math/math.h>


void
task_render(void *arg)
{
	struct roa_renderer_ctx *ctx = (struct roa_renderer_ctx*)arg;
	ROA_ASSERT(ctx);

	/* camera */
	struct roa_renderer_camera camera = ctx->camera[0];

	static roa_mat4 view;
  static roa_mat4 proj;

  /* create mats */
  {
    float aspect = (float)ctx->settings.device_viewport[0] / (float)ctx->settings.device_viewport[1];
		roa_mat4_projection(&proj, camera.field_of_view, camera.near_plane, camera.far_plane, aspect);

		roa_float3 from = roa_float3_import(camera.position);
		roa_float3 at = roa_float3_import(camera.lookat);
		roa_float3 up = roa_float3_import(camera.up);

		roa_mat4_lookat(&view, from, at, up);
	}

}
