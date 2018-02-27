#include <volt/utils/prim_model.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>


void
volt_util_generate_cube(
  volt_vert_desc *desc,
  unsigned desc_count,
  float x_scale,
  float y_scale,
  float z_scale,
  float *out_verts,
  unsigned *out_element_count)
{
  float raw_pos_data[]
  {
    +1.f, -1.f, -1.f,
    +1.f, -1.f, +1.f,
    -1.f, -1.f, +1.f,
    -1.f, -1.f, -1.f,
    +1.f, +1.f, -1.f,
    +1.f, +1.f, +1.f,
    -1.f, +1.f, +1.f,
    -1.f, +1.f, -1.f,
  };

  float raw_normal_data[]
  {
    +0.f, -1.f, +0.f,
    +0.f, +1.f, +0.f,
    +1.f, +0.f, +0.f,
    -0.f, +0.f, +1.f,
    -1.f, -0.f, -0.f,
    +0.f, +0.f, -1.f,
  };

  float raw_uv_data[]
  {
    1.f, 0.f,
    1.f, 1.f,
    0.f, 1.f,
    0.f, 0.f,
  };

  int raw_index[]
  {
    1,0,0, 2,1,0, 3,2,1,
    7,0,1, 6,1,1, 5,2,1,
    4,0,2, 5,1,2, 1,2,2,
    5,0,3, 6,1,3, 2,2,3,
    2,3,4, 6,0,4, 7,1,4,
    0,0,5, 3,1,5, 7,2,5,
    0,3,0, 1,0,0, 3,2,0,
    4,3,1, 7,0,1, 5,2,1,
    0,3,2, 4,0,2, 1,2,2,
    1,3,3, 5,0,3, 2,2,3,
    3,2,4, 2,3,4, 7,1,4,
    4,3,5, 0,0,5, 7,2,5,
  };

  unsigned ele_count = 0;

  const unsigned index_count = ROA_ARR_COUNT(raw_index);
  ROA_ASSERT(index_count % 3 == 0);

  for (unsigned i = 0; i < index_count; i += 3)
  {
    for (unsigned j = 0; j < desc_count; ++j)
    {
      if (desc[j] == VOLT_VERT_POSITION)
      {
        unsigned position_index = raw_index[i + 0] * 3;
        
        if(out_verts)
        {
          out_verts[ele_count + 0] = raw_pos_data[position_index + 0] * x_scale;
          out_verts[ele_count + 1] = raw_pos_data[position_index + 1] * y_scale;
          out_verts[ele_count + 2] = raw_pos_data[position_index + 2] * z_scale;
        }

        ele_count += 3;
      }
      else if (desc[j] == VOLT_NORMAL)
      {
        unsigned tex_c_index = raw_index[i + 2] * 3;

        if(out_verts)
        {
          out_verts[ele_count + 0] = raw_normal_data[tex_c_index + 0];
          out_verts[ele_count + 1] = raw_normal_data[tex_c_index + 1];
          out_verts[ele_count + 2] = raw_normal_data[tex_c_index + 2];
        }

        ele_count += 3;
      }
      else if (desc[j] == VOLT_UV)
      {
        unsigned normal_index = raw_index[i + 1] * 2;

        if(out_verts)
        {
          out_verts[ele_count + 0] = raw_uv_data[normal_index + 0];
          out_verts[ele_count + 1] = raw_uv_data[normal_index + 1];
        }

        ele_count += 2;
      }
    }

    *out_element_count = ele_count;
  }
  
}