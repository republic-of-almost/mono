#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <task/task.h>
#include <roa_math/math.h>
#include <stdio.h>


unsigned
roa_renderer_task_pump(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_task **tasks)
{
  unsigned cam_count = roa_array_size(ctx->camera_id);
  
  roa_array_resize(ctx->mesh_renderpasses, cam_count);

  unsigned i;
  for (i = 0; i < cam_count; ++i)
  {
    const struct roa_renderer_camera *cam = &ctx->camera[i];
    struct mesh_renderpass *pass = &ctx->mesh_renderpasses[i];

    /* camera data */

    /* look at */
    roa_mat4 view;
    {
      roa_float3 at  = roa_float3_import(cam->lookat);
      roa_float3 up  = roa_float3_import(cam->up);
      roa_float3 pos = roa_float3_import(cam->position);

      roa_mat4_lookat(&view, pos, at, up);

      roa_mat4_export(&view, pass->view);
      roa_float3_export(at, pass->camera_pos);
    }

    /* projection */
    roa_mat4 proj;
    {
      float width  = ctx->settings.device_viewport[0];
      float height = ctx->settings.device_viewport[1];
      float aspect = width / height;

      roa_mat4_projection(
        &proj,
        cam->field_of_view,
        cam->near_plane,
        cam->far_plane,
        aspect);

      roa_mat4_export(&proj, pass->projection);
    }

    roa_mat4 view_proj;
    roa_mat4_multiply(&view_proj, &view, &proj);

    /* mesh renderables */
    {
      unsigned mesh_count = roa_array_size(ctx->renderable);
      unsigned j;

      pass->draw_calls = ROA_NULL;
      roa_array_create_with_capacity(pass->draw_calls, mesh_count);
      roa_array_resize(pass->draw_calls, mesh_count);

      for (j = 0; j < mesh_count; ++j)
      {
        const struct roa_renderer_mesh_renderable *rdr = &ctx->renderable[j];
        struct mesh_draw_call *dc = &pass->draw_calls[j];

        roa_mat4 world;
        roa_mat4_import(&world, rdr->world_transform);

        roa_mat4 world_view_proj;
        roa_mat4_multiply(&world_view_proj, &world, &view_proj);

        roa_mat4_export(&world, dc->world);
        roa_mat4_export(&world_view_proj, dc->world_view_projection);
      }
    }
  }

  return 0;
}


void
roa_renderer_task_run(
	struct roa_renderer_task *task)
{
  if(task && task->func)
  {
    roa_renderer_task_func task_func = (roa_renderer_task_func)task->func;
    task_func(task->arg);
  }
}
