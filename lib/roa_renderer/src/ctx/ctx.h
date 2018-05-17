#ifndef ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919
#define ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919


#include <roa_lib/fundamental.h>
#include <roa_renderer/roa_renderer.h>
#include <roa_lib/atomic.h>


#ifdef ROA_RENDERER_API_NOOP
#include <graphics_api/noop/noop.h>
#elif defined ROA_RENDERER_API_GL4
#include <graphics_api/ogl/ogl4.h>
#endif


/* -- [ Raw Data ] -- */


struct renderer_data_desc
{
  roa_atomic_int lock;

  /* array */ uint32_t *camera_ids;
  /* array */ struct roa_renderer_camera *camera_descs;

  /* array */ uint32_t *mesh_rdr_ids;
  /* array */ struct roa_renderer_mesh_renderable *mesh_rdr_descs;

  /* array */ uint32_t *light_ids;
  /* array */ struct roa_renderer_light *light_descs;
};


struct renderer_resource_data_desc
{
  roa_atomic_int lock;

  /* array */ uint64_t *mesh_ids;
  /* array */ struct roa_renderer_mesh_resource *mesh_rsrc_data;

  /* array */ uint64_t *mesh_pending_ids;
  /* array */ struct roa_renderer_mesh_resource *mesh_rsrc_pending_data;

  /* array */ uint64_t *material_ids;
  /* array */ struct roa_renderer_material_resource *mat_descs;
};


/* -- [ Renderpasses ] -- */


struct renderpass_camera
{
  float projection[16];
  float view[16];
  float view_projection[16];
  float position[3];
};


struct renderpass
{
  struct renderpass_camera camera;;
};


struct renderer_renderpass_data
{
  roa_atomic_int lock;

  struct renderpass *rps;
};


struct device_setting_data
{
  roa_atomic_int lock;

  float device_viewport[2];
};


struct roa_renderer_ctx
{
  struct device_setting_data device_settings;
  struct renderer_data_desc renderer_desc;
  struct renderer_resource_data_desc resource_desc;
  struct renderer_renderpass_data renderpass;
  struct graphics_api graphics_api;
};


#endif /* inc guard */
