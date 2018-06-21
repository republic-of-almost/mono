#ifndef ROA_RENDERER_INCLUDED_92B47EA1_1B53_4F49_89DD_135F82A0A2C3
#define ROA_RENDERER_INCLUDED_92B47EA1_1B53_4F49_89DD_135F82A0A2C3


#include <roa_lib/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------------------- [ Type ] -- */


typedef struct roa_renderer_ctx * roa_renderer_ctx_t;

typedef void*(*roa_renderer_alloc)(unsigned bytes);
typedef void(*roa_renderer_free)(void *addr);

#define ROA_RENDERER_TYPE_BYTE 1
#define ROA_RENDERER_TYPE_UBYTE 2
#define ROA_RENDERER_TYPE_SHORT 3
#define ROA_RENDERER_TYPE_USHORT 4
#define ROA_RENDERER_TYPE_UINT 5
#define ROA_RENDERER_TYPE_FLOAT 7


/* ---------------------------------------------------------- [ Lifetime ] -- */


struct roa_renderer_ctx_desc
{
  int i; /* ignore atm */
};


ROA_BOOL
roa_renderer_ctx_create(
	roa_renderer_ctx_t *ctx,
  struct roa_renderer_ctx_desc *desc);


void
roa_renderer_ctx_execute(
  roa_renderer_ctx_t ctx);


void
roa_renderer_ctx_destroy(
  roa_renderer_ctx_t *ctx);


/* ---------------------------------------------------------- [ Settings ] -- */


struct roa_renderer_device_viewport
{
  unsigned width;
  unsigned height;
};


void
roa_renderer_set_device_viewport(
  roa_renderer_ctx_t ctx,
  const struct roa_renderer_device_viewport *vp);


void
roa_renderer_get_device_viewport(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_device_viewport *out_vp);


/* -------------------------------------------------------------- [ Load ] -- */


ROA_BOOL
roa_renderer_load(
  roa_renderer_ctx_t ctx,
  const char *file);


/* ------------------------------------------------------------ [ Camera ] -- */


struct roa_renderer_camera
{
  float near_plane;
  float far_plane;

  float field_of_view;

  float position[3];
  float lookat[3];
  float up[3];
};


ROA_BOOL
roa_renderer_camera_set(
	roa_renderer_ctx_t ctx,
	struct roa_renderer_camera *camera,
	uint32_t camera_id);


ROA_BOOL
roa_renderer_camera_get(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_camera *out_camera,
  uint32_t camera_id);


ROA_BOOL
roa_renderer_camera_clear(
  roa_renderer_ctx_t ctx,
  uint32_t camera_id);


unsigned
roa_renderer_camera_count(
  const roa_renderer_ctx_t ctx);


ROA_BOOL
roa_renderer_camera_exists(
  const roa_renderer_ctx_t ctx,
  uint32_t camera_id);


/* --------------------------------------------------- [ Mesh Renderable ] -- */


struct roa_renderer_decal
{
  float color[3];
  float scale[3];
  float position[3];
  float rotation[4];
};


//struct roa_renderer_mesh_renderable
//{
//  float world_transform[16];
//
//  uint64_t mesh_id;
//
//  struct roa_renderer_decal *decals_lod0;
//  unsigned decals_lod0_count;
//};


ROA_BOOL
roa_renderer_mesh_renderable_create(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id);


ROA_BOOL
roa_renderer_mesh_renderable_destroy(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id);


ROA_BOOL
roa_renderer_mesh_renderable_transform_set(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id,
  float *pos,
  float *scale,
  float *rotation);


ROA_BOOL
roa_renderer_mesh_renderable_transform_get(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id,
  float *out_pos,
  float *out_scale,
  float *out_rotation);


unsigned
roa_renderer_mesh_renderable_count(
  roa_renderer_ctx_t ctx);


ROA_BOOL
roa_renderer_mesh_renderable_exists(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id);


/* ------------------------------------------------ [ Material Resources ] -- */


struct roa_renderer_material_resource
{
  uint32_t shader;
  uint32_t texture;
};


unsigned
roa_renderer_material_resource_count(
  roa_renderer_ctx_t ctx);


/* ---------------------------------------------------- [ Mesh Resources ] -- */


struct roa_renderer_mesh_resource
{
  const char *name;

  float *position_vec3_array;
  float *normal_vec3_array;
  float *texture_coord_vec2_array;
  unsigned vertex_count;

  int index_stride;
  void *index_array;
  unsigned index_count;

  struct roa_renderer_decal *decals_lod0;
  int decals_lod0_count;

  struct roa_renderer_decal *decals_lod1;
  int decals_lod1_count;

  struct roa_renderer_decal *decals_lod2;
  int decals_lod2_count;

  struct roa_renderer_decal *decals_lod3;
  int decals_lod3_count;

  struct roa_renderer_decal *decals_lod4;
  int decals_lod4_count;

  struct roa_renderer_decal *decals_lod5;
  int decals_lod5_count;

  struct roa_renderer_decal *decals_lod6;
  int decals_lod6_count;

  struct roa_renderer_decal *decals_lod7;
  int decals_lod7_count;

  struct roa_renderer_decal *decals_lod8;
  int decals_lod8_count;

  struct roa_renderer_decal *decals_lod9;
  int decals_lod9_count;
};


unsigned
roa_renderer_mesh_resource_count(
  roa_renderer_ctx_t ctx);


uint64_t
roa_renderer_mesh_resource_add(
  roa_renderer_ctx_t ctx,
  const struct roa_renderer_mesh_resource *rsrc);


ROA_BOOL
roa_renderer_mesh_resource_exists(
  roa_renderer_ctx_t ctx,
  const char *name);


/* ------------------------------------------------- [ Texture Resources ] -- */


struct roa_renderer_texture_resource
{
  const char *name;
  int width;
  int height;
  int components;
  void *data;
};


unsigned
roa_renderer_texture_resource_count(
  roa_renderer_ctx_t ctx);


uint64_t
roa_renderer_texture_resource_add(
  roa_renderer_ctx_t ctx,
  const struct roa_renderer_texture_resource *rsrc);


ROA_BOOL
roa_renderer_texture_resource_exists(
  roa_renderer_ctx_t ctx,
  const char *name);


/* -------------------------------------------------------------- [ Task ] -- */


struct roa_renderer_task
{
  void *func;

  void *arg1;
  void *arg2;
  void *arg3;
};


unsigned
roa_renderer_task_pump(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_task **tasks);


void
roa_renderer_task_run(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_task *task);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
