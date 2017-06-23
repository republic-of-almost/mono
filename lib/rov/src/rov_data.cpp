/*
  We use a generic way of handling the data this makes implimenting the api
  support easier.
*/

#include <rov/rov.hpp>
#include <vector>
#include <lib/utilities.hpp>
#include "rov_data.hpp"


namespace ROV_Internal {


uint64_t
rov_curr_material(rovData *data)
{
  /*
    Convert color to uint8_t
  */
  uint8_t red   = (uint8_t)(data->curr_rov_clear_color[0] * 255);
  uint8_t green = (uint8_t)(data->curr_rov_clear_color[1] * 255);
  uint8_t blue  = (uint8_t)(data->curr_rov_clear_color[2] * 255);
  uint8_t alpha = (uint8_t)(data->curr_rov_clear_color[3] * 255);

  /*
    Pack color together
  */
  const uint32_t color = lib::bits::pack8888(red, green, blue, alpha);


  /*
    Pack in the other details
  */
  uint8_t shader_type = data->curr_rov_mesh_shader;
  uint8_t texture_01 = data->curr_rov_textures[0];
  uint8_t texture_02 = data->curr_rov_textures[1];
  uint8_t texture_03 = data->curr_rov_textures[2];

  const uint32_t details = lib::bits::pack8888(shader_type, texture_01, texture_02, texture_03);

  return lib::bits::pack3232(details, color);
}


} // ns
