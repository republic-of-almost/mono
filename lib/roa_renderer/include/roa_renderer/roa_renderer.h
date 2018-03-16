#ifndef ROA_RENDERER_INCLUDED_92B47EA1_1B53_4F49_89DD_135F82A0A2C3
#define ROA_RENDERER_INCLUDED_92B47EA1_1B53_4F49_89DD_135F82A0A2C3


#include <roa_lib/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------------------- [ Type ] -- */


typedef struct roa_renderer_ctx * roa_renderer_ctx_t;


/* ---------------------------------------------------------- [ Lifetime ] -- */


ROA_BOOL
roa_renderer_ctx_create(
	roa_renderer_ctx_t *ctx);


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


/* ------------------------------------------------------------ [ Camera ] -- */

struct roa_renderer_camera
{
	float near_plane;
	float far_plane;
	float field_of_view;

	float position[3];
	float direction[3];
	float up[3];
};


ROA_BOOL
roa_renderer_camera_set(
	roa_renderer_ctx_t ctx,
	struct roa_renderer_camera *camera,
	uint32_t camera_id);


ROA_BOOL
roa_renderer_camera_clear(
	roa_renderer_ctx_t ctx,
	uint32_t camera_id);


/* -------------------------------------------------------- [ Renderable ] -- */


struct roa_renderer_renderable
{
	float world_transform[16];
};


ROA_BOOL
roa_renderer_renderable_set(
	roa_renderer_ctx_t ctx,
	struct roa_renderer_renderable *renderable,
	uint32_t renderable_id);


ROA_BOOL
roa_renderer_renderable_clear(
	roa_renderer_ctx_t ctx,
	uint32_t renderable_id);


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


#ifdef __cpluplus
} /* extern */
#endif


#endif /* inc guard */
