#ifndef MODEL_INCLUDED_1F4F06F2_C04E_483C_8411_88D3BFDA1333
#define MODEL_INCLUDED_1F4F06F2_C04E_483C_8411_88D3BFDA1333


#ifdef __cplusplus
extern "C" {
#endif


typedef enum _volt_vert_desc {

  VOLT_VERT_POSITION,
  VOLT_NORMAL,
  VOLT_UV,

} volt_vert_desc;


void
volt_util_generate_cube(
  volt_vert_desc *desc,
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