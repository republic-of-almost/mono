#include <game/environment.hpp>
#include <lib/utilities.hpp>
#include <math/math.hpp>


namespace {

constexpr uint32_t env_count = 200;

} // anon ns


namespace Game {


namespace {


inline void
think(ROA::Object node)
{
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
    
    const ROA::Material mat(
      name,
      ROA::Color(rgba)
    );
    LIB_ASSERT(mat.is_valid());
    
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
  
  ROA::Model::load("mesh/unit_bev_cube.obj");
  ROA::Mesh mesh("Unit_bev_cube");

  // Child nodes
  for(uint32_t i = 0; i < env_count; ++i)
  {
    ROA::Object node;
    node.set_name("Env");
    node.set_parent(env->entity);
    
    char name[16]{};
    sprintf(name, "Env%dMat", i);
    
    ROA::Material mat(name);
    mat.set_color(ROA::Color(0xFF00FFFF));
    
    ROA::Renderable renderable;
    renderable.set_material(mat);
    renderable.set_mesh(mesh);
    
    node.set_renderable(renderable);
  }
  
  // Callbacks
  {
    ROA::Logic logic;
    
    logic.update_func(think);
    
    env->entity.set_logic(logic);
  }
}


} // ns
