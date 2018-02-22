#include <volt/utils/prim_model.h>



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
  unsigned *out_index_count)
{
  float x_start = 0.f - (x_size * 0.5f);
  float x_step = x_size / (float)subdivisions;

  float y_start = 0.f - (y_size * 0.5f);
  float y_step = y_size / (float)subdivisions;

  float raw_pos_data[]
  {
    0.f,
  };

  float raw_normal_data[]
  {
    0.f,
  };

  float raw_uv_data[]
  {
    0.f,
  };

  for (int x = 0; x < subdivisions; ++x)
  {
    for (int y = 0; y < subdivisions; ++y)
    {
      
    }
  }
}