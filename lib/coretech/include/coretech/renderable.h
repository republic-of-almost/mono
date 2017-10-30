#ifndef RENDERABLE_INCLUDED_74667A85_F5AB_4444_A3F5_9F0C61DEEF5B
#define RENDERABLE_INCLUDED_74667A85_F5AB_4444_A3F5_9F0C61DEEF5B


#include <coretech/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ----------------------------------------------- [ Renderable Lifetime ] -- */


uint64_t      ct_renderable_create(uint64_t obj_id);
void          ct_renderable_destroy(uint64_t rdr_id);


/*  -------------------------------------------- [ Renderable Attributes ] -- */


void          ct_renderable_set_mesh(uint64_t rdr_id, uint64_t mesh_id);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
