#include <game/ground.hpp>
#include <assets/cube_mesh.hpp>
#include <nil/data/transform.hpp>
#include <nil/data/material.hpp>
#include <nil/data/mesh_resource.hpp>
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

    const float scale[] = {10.f, 0.25f, 10.f};
    memcpy(transform.scale, scale, sizeof(transform.scale));
    
    const float position[] = {0.f, -0.3f, 0.f};
    memcpy(transform.position, position, sizeof(transform.position));
    
    const float rotation[] = {0.f, 0.f, 0.f, 1.f};
    memcpy(transform.rotation, rotation, sizeof(transform.rotation));
    
    Nil::Data::set(ground->entity, transform);
  }
  
  // Material
  {
    Nil::Data::Material material{};
    
    const float color[] = {0.5f, 0.f, 0.5f, 1.f};
    memcpy(material.color, color, sizeof(material.color));
    
    LOG_TODO_ONCE("Get rid of magic number");
    
    material.shader = Nil::Data::Material::FULLBRIGHT;
    
    Nil::Data::set(ground->entity, material);
  }
  
  // Mesh Resource
  {
    Nil::Data::Mesh_resource mesh{};

    mesh.position_vec3 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_positions));
    memcpy(mesh.position_vec3, Nil_ext::Mesh::bev_cube_positions, sizeof(Nil_ext::Mesh::bev_cube_positions));

    mesh.normal_vec3 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_normals));
    memcpy(mesh.normal_vec3, Nil_ext::Mesh::bev_cube_normals, sizeof(Nil_ext::Mesh::bev_cube_normals));

    mesh.texture_coords_vec2 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_texture_coords));
    memcpy(mesh.texture_coords_vec2, Nil_ext::Mesh::bev_cube_texture_coords, sizeof(Nil_ext::Mesh::bev_cube_texture_coords));

    mesh.count = Nil_ext::Mesh::bev_cube_mesh_vert_count;

    Nil::Data::set(ground->entity, mesh);
  }
}


} // ns
