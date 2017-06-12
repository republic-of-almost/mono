#include <game/environment.hpp>
#include <nil/data/material.hpp>
#include <nil/data/transform.hpp>
#include <nil/data/logic.hpp>
#include <lib/assert.hpp>
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
    
    float rgba[] = {1.f, (y / 10.f) * 1.f, 1.f, 1.f};
    
    Nil::Data::Material material{};
    Nil::Data::get(child, material);
    
    memcpy(material.color, rgba, sizeof(material.color));
    
    Nil::Data::set(child, material);
    
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
  
  // Child nodes
  for(uint32_t i = 0; i < env_count; ++i)
  {
    Nil::Node node;
    node.set_name("Env");
    node.set_parent(env->entity);
    
    Nil::Data::Material material{};
    Nil::Data::set(node, material);
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
