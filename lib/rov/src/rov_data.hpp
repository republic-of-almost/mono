#ifndef ROV_PLATFORM_EXEC_INCLUDED_A4B15F54_F56E_45BE_8E34_D451F985452C
#define ROV_PLATFORM_EXEC_INCLUDED_A4B15F54_F56E_45BE_8E34_D451F985452C


#include <rov/rov.hpp>
#include <lib/array.hpp>


namespace ROV_Internal
{
  struct Camera
  {
    rovMat4 view;
    rovMat4 proj;
  };

  struct rovMaterial
  {
    uint64_t material;
    size_t draw_calls;
  };

  struct rovDrawCall
  {
    uint32_t mesh;
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

    uint32_t clear_flags;

    lib::array<rovMaterial> materials;
    lib::array<rovDrawCall> draw_calls;
    
    lib::array<rovLineDrawCall> line_draw_calls;
  };



/*
  Defined by the API.
*/
void
api_execute(const rovRenderPass passes[], const size_t pass_count);


uint32_t
api_light_buffer(const rovLight lights[], const size_t light_count);


}



#endif // inc guard
