#include <roa_renderer/roa_renderer.h>
#include <ctx/ctx.h>
#include <roa_lib/array.h>
#include <task/task.h>
#include <roa_math/math.h>
#include <stdio.h>
#include <roa_lib/spin_lock.h>


unsigned
roa_renderer_task_pump(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_task **tasks)
{
  roa_spin_lock_aquire(&ctx->renderer_desc.lock);
  roa_spin_lock_aquire(&ctx->renderpass.lock);
  roa_spin_lock_aquire(&ctx->device_settings.lock);
  
  unsigned cam_count = roa_array_size(ctx->renderer_desc.camera_ids);
  roa_array_resize(ctx->renderpass.rps, cam_count);

  int i = 0;
  for (i = 0; i < cam_count; ++i)
  {
    struct roa_renderer_camera cam = ctx->renderer_desc.camera_descs[i];
    struct renderpass *rp = &ctx->renderpass.rps[i];

    /* calculate camera mats */
    roa_mat4 view;
    roa_float3 pos = roa_float3_import(cam.position);
    roa_float3 up = roa_float3_import(cam.up);
    roa_float3 at = roa_float3_import(cam.lookat);

    roa_mat4_lookat(&view, pos, at, up);
    unsigned view_size = sizeof(rp->camera.view);
    memcpy(rp->camera.view, view.data, view_size);

    roa_mat4 proj;
    float ratio = (float)ctx->device_settings.device_viewport[0] / (float)ctx->device_settings.device_viewport[1];
    
    roa_mat4_projection(&proj, cam.field_of_view, cam.near_plane, cam.far_plane, ratio);
    unsigned proj_size = sizeof(rp->camera.projection);
    memcpy(rp->camera.projection, proj.data, proj_size);

    roa_mat4 view_proj;
    roa_mat4_multiply(&view_proj, &view, &proj);

    unsigned view_proj_size = sizeof(rp->camera.view_projection);
    memcpy(rp->camera.view_projection, &view_proj, view_proj_size);

    unsigned pos_size = sizeof(rp->camera.position);
    memcpy(rp->camera.position, cam.position, pos_size);

    rp->draw_calls = 0;
    unsigned dc_count = roa_array_size(ctx->renderer_desc.mesh_rdr_ids);
    roa_array_create_with_capacity(rp->draw_calls, dc_count);
    unsigned j;

    for (j = 0; j < dc_count; ++j)
    {
      struct roa_renderer_mesh_renderable rdr = ctx->renderer_desc.mesh_rdr_descs[j];

      struct draw_call dc;
      memcpy(dc.world, rdr.world_transform, sizeof(dc.world));

      roa_mat4 world;
      roa_mat4_import(&world, rdr.world_transform);

      roa_mat4 wvp;
      roa_mat4_multiply(&wvp, &world, &view_proj);
      memcpy(dc.wvp, wvp.data, sizeof(dc.wvp));

      roa_array_push(rp->draw_calls, dc);
    }
  }

  roa_spin_lock_release(&ctx->renderer_desc.lock);
  roa_spin_lock_release(&ctx->renderpass.lock);
  roa_spin_lock_release(&ctx->device_settings.lock);

  return 0;
}


void
roa_renderer_task_run(
	struct roa_renderer_task *task)
{

}
