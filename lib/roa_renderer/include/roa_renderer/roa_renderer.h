#ifndef ROA_RENDERER_INCLUDED_
#define ROA_RENDERER_INCLUDED_


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


/* ------------------------------------------------------------ [ Camera ] -- */




/* -------------------------------------------------------- [ Renderable ] -- */


struct roa_renderer_renderable
{
	int camera_mask;
	int renderer_id;
	int cast_shadow;
	int recive_shadow;
};


/* ----------------------------------------------------- [ Mesh Renderer ] -- */



/* ----------------------------------------------------- [ Mesh Resource ] -- */



/* -------------------------------------------------------------- [ Task ] -- */


typedef struct roa_renderer_task * roa_renderer_task_t;


unsigned
roa_renderer_task_pump(
	roa_renderer_ctx_t ctx,
  roa_renderer_task_t *tasks);


void
roa_renderer_task_run(
	roa_renderer_task_t task);


#ifdef __cpluplus
} /* extern */
#endif


#endif /* inc guard */
