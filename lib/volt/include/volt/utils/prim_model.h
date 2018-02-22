#ifndef MODEL_INCLUDED_1F4F06F2_C04E_483C_8411_88D3BFDA1333
#define MODEL_INCLUDED_1F4F06F2_C04E_483C_8411_88D3BFDA1333


#ifdef __cplusplus
extern "C" {
#endif


typedef enum _volt_vert_desc {

  VOLT_VERT_POS3,
  VOLT_VERT_POS2,
  VOLT_NORMAL,
  VOLT_UV,

} volt_vert_desc;


void
volt_util_generate_plane(
  volt_vert_desc *desc,
  unsigned desc_count,
  unsigned x_size,
  unsigned y_size,
  unsigned subdivisions,
  float **out_verts,
  unsigned *out_vert_count,
  float **out_index,
  unsigned *out_index_count);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */