#ifndef ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919
#define ROA_RENDERER_CTX_INCLUDED_B747703D_4DBC_4BC2_A813_2D060A864919


#include <roa_lib/fundamental.h>
#include <roa_renderer/roa_renderer.h>
#include <volt/volt.h>
#include <roa_lib/atomic.h>


struct mesh_rendering_data
{
  int i;
};


struct mesh_camera_data
{
  int i;
};


struct roa_mesh_rsrc
{
	roa_atomic_int lock;

	/* array */ uint64_t *pending_ids;

	/* array */ uint64_t *ids;
	/* array */ struct roa_renderer_mesh_resource *rsrc;
};


struct roa_device_settings
{
  unsigned device_viewport[2];
};


struct roa_renderer_ctx
{
	/* data */

	/* array */ uint32_t *camera_id;
	/* array */ struct roa_renderer_camera *camera;

	/* array */ uint32_t *renderable_id;
	/* array */ struct roa_renderer_renderable *renderable;

	struct roa_mesh_rsrc mesh_rsrc;

	/* job data */
  
  /* array */ struct mesh_rendering_data *mesh_rendering_data;
  /* array */ struct mesh_camera_data *mesh_camera_data;

  /* array */ struct roa_renderer_task *tasks;

	/* low level renderer */
  volt_ctx_t volt_ctx;

	/* test stuff */
	volt_uniform_t world_data;
	volt_uniform_t proj_data;
	volt_uniform_t view_data;
	volt_texture_t texture_1;
	volt_texture_t texture_2;
	volt_rasterizer_t rasterizer;
	volt_input_t input_format;
	volt_vbo_t vbo;
	volt_ibo_t ibo;
	volt_program_t program;
	
	ROA_BOOL render;

  /* settings */
  struct roa_device_settings settings;
};


#endif /* inc guard */
