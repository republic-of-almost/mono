#include <game/environment.hpp>
#include <assets/cube_mesh.hpp>
#include <nil/resource/resource.hpp>
#include <nil/data/data.hpp>
#include <lib/utilities.hpp>
#include <math/math.hpp>


namespace {

constexpr uint32_t env_count = 200;

} // anon ns


namespace Game {


namespace {


inline void
think(Nil::Node node, uintptr_t user_data)
{
  static float env_time = 0.f;
  env_time += 0.16f * 0.005f;

  for(uint32_t i = 0; i < env_count; ++i)
  {
    Nil::Node child = node.get_child(i);
    
    constexpr float radius = 15.f;
    const float time = i;
    const float half_root_two = math::root_two() * 0.5f;
    
    const float y = i / 20.f;
    const float x = math::clamp(math::cos(time + env_time), -half_root_two, +half_root_two) * (radius + y);
    const float z = math::clamp(math::sin(time + env_time), -half_root_two, +half_root_two) * (radius + y);
    
    float rgba[] = {1.f, (y / 10.f), 1.f, 1.f};
    
    char name[16]{};
    sprintf(name, "Env%dMat", i);
    
    Nil::Resource::Material mat{};
    const bool found = Nil::Resource::find_by_name(name, mat);
    LIB_ASSERT(found);
    
    mat.color = lib::color::init(rgba);
    
    Nil::Resource::load(name, mat);
    
    const float default_scale = 2.f;
    const float scale_up = default_scale * ((float)i / 10.f);
    
    Nil::Data::Transform transform{};
    Nil::Data::get(child, transform);
    
    transform.scale[0] = default_scale;
    transform.scale[1] = scale_up;
    transform.scale[2] = default_scale;
    
    transform.position[0] = x;
    transform.position[1] = y;
    transform.position[2] = z;
    
    Nil::Data::set(child, transform);
  }
}


} // anon ns


void
setup(Environment *env)
{
  LIB_ASSERT(env);
  
  env->entity.set_name("Environment");
  
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

    Nil::Resource::load("EnvCube", mesh);
  }  
  
  // Child nodes
  for(uint32_t i = 0; i < env_count; ++i)
  {
    Nil::Node node;
    node.set_name("Env");
    node.set_parent(env->entity);

    char name[16]{};
    sprintf(name, "Env%dMat", i);
    
    Nil::Resource::Material material{};
    material.color = 0xFF0000FF;
    Nil::Resource::load(name, material);
    
    Nil::Data::Renderable renderable;
    renderable.material_id = material.id;
    renderable.mesh_id = mesh.id;
    Nil::Data::set(node, renderable);
  }
  
  // Callbacks
  {
    Nil::Data::Logic logic{};
    
    logic.logic_id = 1;
    logic.user_data = (uintptr_t)env;
    
    logic.think_01 = think;
    
    Nil::Data::set(env->entity, logic);
  }
}


} // ns
