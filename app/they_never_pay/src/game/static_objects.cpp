#include <game/static_objects.hpp>
#include <nil/data/transform.hpp>
#include <nil/data/renderable.hpp>
#include <nil/data/mesh.hpp>
#include <nil/data/mesh_resource.hpp>
#include <lib/logging.hpp>
#include <math/general/general.hpp>
#include <assets/cube_mesh.hpp>
#include <data/data.hpp>
#include <stdlib.h>


namespace Game {


void
setup(Static_objects *obj)
{
  obj->object.set_name("Ground");
  
  // Transform
  {
    Nil::Data::Transform transform{};

    constexpr float floor_scale = 30.f;

    const float scale[] = {floor_scale, 0.25f, floor_scale * math::g_ratio()};
    memcpy(transform.scale, scale, sizeof(transform.scale));
    
    const float position[] = {0.f, -0.3f, 0.f};
    memcpy(transform.position, position, sizeof(transform.position));
    
    const float rotation[] = {0.f, 0.f, 0.f, 1.f};
    memcpy(transform.rotation, rotation, sizeof(transform.rotation));
    
    Nil::Data::set(obj->object, transform);
  }
  
  // Material
  {
    Nil::Data::Renderable material{};
    
    const float color[] = {0.5f, 0.f, 0.5f, 1.f};
    memcpy(material.color, color, sizeof(material.color));
    
    LOG_TODO_ONCE("Get rid of magic number");
    
    material.shader = Nil::Data::Renderable::FULLBRIGHT;
    
    Nil::Data::set(obj->object, material);
  }
  
  // Mesh Instance
  {
    Nil::Data::Mesh mesh{};
    mesh.mesh_id = (uint32_t)Game_asset::CUBE;
    
    Nil::Data::set(obj->object, mesh);
  }
}



} // ns
