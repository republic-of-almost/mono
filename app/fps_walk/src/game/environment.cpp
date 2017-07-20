#include <game/environment.hpp>
#include <nil/resource/resource.hpp>
#include <nil/data/data.hpp>
#include <lib/utilities.hpp>
#include <math/math.hpp>
#include <nil/node.hpp>


namespace {

constexpr uint32_t env_count = 200;

} // anon ns


namespace Game {


namespace {


inline void
think(uint32_t node_id, uintptr_t user_data)
{
  ROA::Object node(node_id);

  static float env_time = 0.f;
  env_time += 0.16f * 0.005f;
  
  const size_t child_count = node.get_child_count();

  for(size_t i = 0; i < child_count; ++i)
  {
    ROA::Object child = node.get_child(i);
    
    constexpr float radius = 15.f;
    const float time = i;
    const float half_root_two = math::root_two() * 0.5f;
    const float offset = time + env_time;
    
    const float y = i / 20.f;
    const float x = math::clamp(math::cos(offset), -half_root_two, +half_root_two) * (radius + y);
    const float z = math::clamp(math::sin(offset), -half_root_two, +half_root_two) * (radius + y);
    
    float rgba[] = {1.f, (y / 10.f), 1.f, 1.f};
    
    char name[32]{};
    sprintf(name, "Env%zuMat", i);
    
    ROA::Material mat(name);
    LIB_ASSERT(mat.is_valid());
    
    mat.set_color(ROA::Color(rgba));
    
    const float default_scale = 4.f;
    const float scale_up = default_scale * ((float)i / 10.f);
    
    ROA::Transform transform = child.get_transform();
    transform.set_scale(ROA::Vector3(default_scale, scale_up, default_scale));
    transform.set_position(ROA::Vector3(x, y, z));
  }
}


} // anon ns


void
setup(Environment *env)
{
  LIB_ASSERT(env);
  
  env->entity.set_name("Environment");
  
  // Mesh Resource
  Nil::Resource::Model::load(
    Nil::Resource::directory("mesh/unit_bev_cube.obj")
  );
  
//  Nil::Resource::Mesh mesh{};
//  Nil::Resource::find_by_name("Unit_bev_cube", mesh);

  ROA::Mesh mesh("Unit_bev_cube");

  // Child nodes
  for(uint32_t i = 0; i < env_count; ++i)
  {
//    Nil::Node node;
    ROA::Object node;
    node.set_name("Env");
    node.set_parent(env->entity);

    char name[16]{};
    sprintf(name, "Env%dMat", i);
    
    ROA::Material mat(name);
    mat.set_color(ROA::Color(0xFF0000FF));
    
//    Nil::Resource::Material material{};
//    material.color = 0xFF0000FF;
//    Nil::Resource::load(name, material);

    ROA::Renderable renderable;
    renderable.set_material(mat);
    renderable.set_mesh(mesh);
    
    node.set_renderable(renderable);

//    Nil::Data::Renderable renderable;
//    renderable.material_id = material.id;
//    renderable.mesh_id = mesh.id;
//    Nil::Data::set(node, renderable);
  }
  
  // Callbacks
  {
    Nil::Data::Logic logic{};
    
    logic.logic_id = 1;
    logic.user_data = (uintptr_t)env;
    
    logic.think_01 = think;
    
    Nil::Node set_node(env->entity.get_instance_id());
    
    Nil::Data::set(set_node, logic);
  }
}


} // ns
