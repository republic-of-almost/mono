#ifndef SCRATCH_INCLUDED_1F4F06F2_C04E_483C_8411_88D3BFDA1333
#define SCRATCH_INCLUDED_1F4F06F2_C04E_483C_8411_88D3BFDA1333



#ifdef __cplusplus
extern "C" {
#endif


typedef enum _geom_vert_desc {

  GEOM_VERT_POSITION,
  GEOM_NORMAL,
  GEOM_UV,

} geom_vert_desc;


void
geometry_generate_cube(
  geom_vert_desc *desc,
  unsigned desc_count,
  float x_scale,
  float y_scale,
  float z_scale,
  float *out_verts,
  unsigned *out_element_count);


#ifdef __cplusplus
} /* extern */
#endif




#endif /* inc guard */

