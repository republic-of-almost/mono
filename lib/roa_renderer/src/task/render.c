#include <task/task.h>
#include <volt/volt.h>
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

	unsigned renderable_count = roa_array_size(ctx->renderable);

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

  unsigned rdr_count = roa_array_size(ctx->renderable);
  unsigned i;

  volt_renderpass_t rp;
  volt_renderpass_create(ctx->volt_ctx, &rp, "cube", ROA_NULL);

  /* bind program */
  volt_renderpass_bind_program(rp, ctx->program);

  /* bind formatting */
  volt_renderpass_bind_input_format(rp, ctx->input_format);
  volt_renderpass_bind_rasterizer(rp, ctx->rasterizer);

  volt_uniform_update(ctx->volt_ctx, ctx->view_data, view.data);
  volt_uniform_update(ctx->volt_ctx, ctx->proj_data, proj.data);

  for (i = 0; i < rdr_count; ++i)
  {
    volt_uniform_update(ctx->volt_ctx, ctx->world_data, ctx->renderable[i].world_transform);

    volt_renderpass_set_viewport(rp, 0, 0, ctx->settings.device_viewport[0], ctx->settings.device_viewport[1]);

    /* bind buffers */
    volt_renderpass_bind_vertex_buffer(rp, ctx->vbo);
    /*volt_renderpass_bind_index_buffer(rp, ctx->ibo);*/
    volt_renderpass_bind_texture_buffer(rp, ctx->texture_1, "texKitten");
    volt_renderpass_bind_texture_buffer(rp, ctx->texture_2, "texPuppy");

    volt_renderpass_bind_uniform(rp, ctx->proj_data, "proj");
    volt_renderpass_bind_uniform(rp, ctx->view_data, "view");
    volt_renderpass_bind_uniform(rp, ctx->world_data, "model");

    /* draw */
    volt_renderpass_draw(rp);
  }

  volt_renderpass_commit(ctx->volt_ctx, &rp);

  
}
