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


/* ---------------------------------------------------------- [ Lifetime ] -- */


struct roa_renderer_ctx_desc
{
  roa_renderer_alloc alloc;
  roa_renderer_free free;

  roa_renderer_alloc task_alloc;
  roa_renderer_free task_free;
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


void
roa_renderer_ctx_lock(
	roa_renderer_ctx_t ctx);


void
roa_renderer_ctx_unlock(
	roa_renderer_ctx_t ctx);


/* ---------------------------------------------------------- [ Settings ] -- */


void
roa_renderer_set_device_resolution(
  roa_renderer_ctx_t ctx,
  unsigned width,
  unsigned height);


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


/* ---------------------------------------------------- [ Mesh Renderable ] -- */


struct roa_renderer_mesh_renderable
{
	float world_transform[16];

	uint64_t mesh_id;
  uint64_t material_id;
};


ROA_BOOL
roa_renderer_mesh_renderable_set(
	roa_renderer_ctx_t ctx,
	struct roa_renderer_mesh_renderable *renderable,
	uint32_t renderable_id);


ROA_BOOL
roa_renderer_mesh_renderable_get(
  roa_renderer_ctx_t ctx,
  struct roa_renderer_mesh_renderable *out_renderable,
  uint32_t renderable_id);


ROA_BOOL
roa_renderer_mesh_renderable_clear(
	roa_renderer_ctx_t ctx,
	uint32_t renderable_id);


unsigned
roa_renderer_mesh_renderable_count(
  roa_renderer_ctx_t ctx);


ROA_BOOL
roa_renderer_mesh_renderable_exists(
  roa_renderer_ctx_t ctx,
  uint32_t renderable_id);


/* ---------------------------------------------------- [ Mesh Resources ] -- */


struct roa_renderer_mesh_resource
{
	const char *name;

	float *vert_vec3_array;
	float *normal_vec3_array;
	float *texture_coord_vec2_array;

	unsigned vert_count;
};


uint64_t
roa_renderer_mesh_resource_add(
	roa_renderer_ctx_t ctx,
	const struct roa_renderer_mesh_resource *rsrc);


/* -------------------------------------------------------------- [ Task ] -- */


struct roa_renderer_task
{
  void *func;
  void *arg;
};


unsigned
roa_renderer_task_pump(
	roa_renderer_ctx_t ctx,
  struct roa_renderer_task **tasks);


void
roa_renderer_task_run(
  struct roa_renderer_task *task);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
