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


struct mesh_draw_call
{
  float world[16];
  float world_view_projection[16];
};


struct mesh_renderpass
{
  float projection[16];
  float view[16];
  float camera_pos[3];

  struct mesh_draw_call *draw_calls;
};


struct renderer_alloc
{
  roa_renderer_alloc alloc;
  roa_renderer_free free;

  roa_renderer_alloc frame_alloc;
  roa_renderer_free frame_free;
};


struct roa_mesh_rsrc
{
	roa_atomic_int lock;

	/* array */ uint64_t *create_ids;

	/* array */ uint64_t *ids;
	/* array */ struct roa_renderer_mesh_resource *rsrc;
};


struct roa_device_settings
{
  float device_viewport[2];
};


struct roa_renderer_ctx
{
  struct renderer_alloc mem;

	/* data */

	/* array */ uint32_t *camera_id;
	/* array */ struct roa_renderer_camera *camera;

	/* array */ uint32_t *renderable_id;
	/* array */ struct roa_renderer_mesh_renderable *renderable;

	struct roa_mesh_rsrc mesh_rsrc;

  /* array */ struct mesh_renderpass *mesh_renderpasses;

  /* settings */
  struct roa_device_settings settings;

  struct graphics_api gfx_api;
};


#endif /* inc guard */
