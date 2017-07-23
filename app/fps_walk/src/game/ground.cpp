#include <game/ground.hpp>
#include <nil/data/data.hpp>
#include <nil/resource/resource.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>


namespace Game {


void
setup(Ground *ground)
{
  ground->entity.set_name("Ground");
  
  // Transform
  {
    Nil::Data::Transform transform{};

    const float scale[] = {10.f, 3.f, 10.f};
    memcpy(transform.scale, scale, sizeof(transform.scale));
    
    const float position[] = {0.f, -0.3f, 0.f};
    memcpy(transform.position, position, sizeof(transform.position));
    
    const float rotation[] = {0.f, 0.f, 0.f, 1.f};
    memcpy(transform.rotation, rotation, sizeof(transform.rotation));
    
    Nil::Data::set(ground->entity, transform);
  }
  
  // Material
  Nil::Resource::Material material{};
  {
    material.color = 0x880088FF;
    
    Nil::Resource::load("GroundMat", material);
  }
  
  // Mesh Resource
  Nil::Resource::Model::load(
    Nil::Resource::directory("mesh/unit_bev_cube.obj")
  );
  
  Nil::Resource::Mesh mesh{};
  Nil::Resource::find_by_name("Unit_bev_cube", mesh);
  
  // Renderable
  {
    Nil::Data::Renderable renderable;
    renderable.material_id = material.id;
    renderable.mesh_id = mesh.id;
    
    Nil::Data::set(ground->entity, renderable);
  }
}


} // ns
