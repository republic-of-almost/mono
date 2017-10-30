#ifndef TRANSFORM_INCLUDED_4FCDA8C0_2E78_46D1_A10D_D62B0F960149
#define TRANSFORM_INCLUDED_4FCDA8C0_2E78_46D1_A10D_D62B0F960149


#include <coretech/fundamental.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------ [ Transform Lifetime ] -- */


uint64_t    ct_transform_create(uint64_t object_id);
void        ct_transform_destroy(uint64_t transform_id);


/* ---------------------------------------------- [ Transform Attributes ] -- */


void        ct_transform_set_local_position(uint64_t transform_id, float *vec3);
void        ct_transform_get_local_position(uint64_t transform_id, float **out_vec3);

void        ct_transform_set_local_scale(uint64_t transform_id, float *vec3);
void        ct_transform_get_local_scale(uint64_t transform_id, float **out_vec3);

void        ct_transform_set_local_rotation(uint64_t transform_id, float *vec4);
void        ct_transform_get_local_rotation(uint64_t transform_id, float **out_vec4);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
