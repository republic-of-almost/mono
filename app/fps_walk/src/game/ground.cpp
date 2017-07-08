#include <game/ground.hpp>
#include <assets/cube_mesh.hpp>
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

    const float scale[] = {10.f, 0.25f, 10.f};
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
    material.color = 0x888888FF;
    
    LOG_TODO_ONCE("Get rid of magic number");
    
    material.shader_type = Nil::Resource::Material::FULLBRIGHT;
    
    Nil::Resource::load("CubeMat", material);
  }
  
  // Mesh Resource
  Nil::Resource::Mesh mesh{};
  {
    mesh.position_vec3 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_positions));
    memcpy(mesh.position_vec3, Nil_ext::Mesh::bev_cube_positions, sizeof(Nil_ext::Mesh::bev_cube_positions));

    mesh.normal_vec3 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_normals));
    memcpy(mesh.normal_vec3, Nil_ext::Mesh::bev_cube_normals, sizeof(Nil_ext::Mesh::bev_cube_normals));

    mesh.texture_coords_vec2 = (float*)malloc(sizeof(Nil_ext::Mesh::bev_cube_texture_coords));
    memcpy(mesh.texture_coords_vec2, Nil_ext::Mesh::bev_cube_texture_coords, sizeof(Nil_ext::Mesh::bev_cube_texture_coords));

    mesh.count = Nil_ext::Mesh::bev_cube_mesh_vert_count;

    Nil::Resource::load("Cube", mesh);
  }
  
  // Renderable
  {
    Nil::Data::Renderable renderable;
    renderable.material_id = material.id;
    renderable.mesh_id = mesh.id;
    
    Nil::Data::set(ground->entity, renderable);
  }
}


} // ns
