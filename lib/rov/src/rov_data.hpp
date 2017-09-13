#ifndef ROV_PLATFORM_EXEC_INCLUDED_A4B15F54_F56E_45BE_8E34_D451F985452C
#define ROV_PLATFORM_EXEC_INCLUDED_A4B15F54_F56E_45BE_8E34_D451F985452C


#include <rov/rov.hpp>
#include <lib/array.hpp>
#include <vector>


namespace ROV_Internal {


struct rovMaterial
{
  uint64_t material;
  size_t draw_calls;
};


struct rovDrawCall
{
  uint32_t mesh;
  uint32_t index;
  rovMat4 world;
};


struct rovLineDrawCall
{
  rovVec3 start;
  rovVec3 end;
  rovVec3 color;
};


struct rovRenderPass
{
  rovMat4 view;
  rovMat4 proj;
  rovViewport viewport;
  uint32_t light_buffer;
  rovVec3 eye_position;

  uint32_t clear_flags;
  uint32_t clear_color;

  uint32_t render_target;

  lib::array<rovMaterial> materials;
  lib::array<rovDrawCall> draw_calls;
  
  lib::array<rovLineDrawCall> line_draw_calls;
  
  rovRasterizer rasterizer;
};



constexpr uint32_t rov_max_textures = 3;


struct rovData
{
  rovRasterizer   curr_rov_rasterizer;
  rovVec4         curr_rov_clear_color;
  rovVec4         curr_rov_color;
  uint8_t         curr_rov_textures[rov_max_textures]{0};
  uint32_t        curr_rov_mesh;
  uint32_t        curr_rov_index;
  uint8_t         curr_rov_mesh_shader = rovShader_Fullbright;
  uint32_t        curr_rov_framebuffer;

  std::vector<ROV_Internal::rovRenderPass> rov_render_passes;
};


uint64_t
rov_curr_material(rovData *data);


}


#endif // inc guard
