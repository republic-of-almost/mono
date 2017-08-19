#include <data/data.hpp>
#include <nil/resource/resource.hpp>
#include <nil/data/window.hpp>
#include <nil/data/developer.hpp>
#include <nil/data/mouse.hpp>
#include <nil/data/light.hpp>
#include <nil/data/keyboard.hpp>
#include <nil/data/transform.hpp>
#include <math/vec/vec3.hpp>
#include <lib/array.hpp>

#include <nil/data/renderable.hpp>

#include <lib/model.hpp>
#include <stb/stb_image.h>
#include <math/geometry/misc.hpp>
#include <roa/roa.hpp>
#include <game/dynamic_object.hpp>


namespace Game_data {


namespace
{
  lib::array<float> line_info;
}


void
load_assets()
{
  ROA::Object assets = get_assets();

//  Nil::Node assets = get_assets();

  // Load static assets
  {
    ROA::Object asset;
  
    asset.set_parent(assets);
    asset.set_name("Static");
    
    ROA::Scene::load(asset, "mesh/static.obj");
  }
  
  // Load dynamic assets
  {
    ROA::Object asset;
    asset.set_parent(assets);
    asset.set_name("Dynamic");
    
    ROA::Scene::load(asset, "mesh/dynamic.obj");
    
    const size_t children = asset.get_child_count();
    
    for(size_t i = 0; i < children; ++i)
    {
      ROA::Object child = asset.get_child(i);
    
      Dynamic_object::Data *obj = Dynamic_object::get_dyn_obj();
      Dynamic_object::setup(obj);
      
      if(obj)
      {
        child.set_user_data((uintptr_t)obj);
      
        ROA::Logic log;
        child.set_logic(log);
        
        log = child.get_logic();
        log.set_update_func(Dynamic_object::update);
        log.set_message_func(Dynamic_object::message_handler);
      }
    }
  }
  
  // Load lights
  {
    const lib::model lights = lib::model_import::load_obj_from_file(
      Nil::Resource::directory("mesh/lights.obj")
    );
  
    ROA::Object asset;
    asset.set_parent(assets);
    asset.set_name("Lights");
    
    for(uint32_t i = 0; i < lights.mesh_count; ++i)
    {
      ROA::Object child;
      child.set_parent(asset);
      child.set_name(lights.name[i]);
      
      ROA::Transform trans = child.get_transform();
      
      const math::vec3 origin = math::get_vec3_origin(lights.verts[i], lights.triangle_count[i]);
      trans.set_position(ROA::Vector3(origin.data));
      
      child.set_transform(trans);
      
      ROA::Light light;
      light.set_type(ROA::Light_type::POINT);
      light.set_constant_atten(0.01f);
      light.set_linear_atten(0.014f);
      light.set_exponential_atten(0.014f);
      
      child.set_light(light);      
    }
  }
}


void
setup()
{
  // Window
  {
    Nil::Node node = get_app();
    node.set_name("App");
    
    Nil::Data::Window win{};
    constexpr char title[] = "App";
    strcat(win.title, title);
    
    Nil::Data::set(node, win);
  }

  // Debug Lines
  {
    Nil::Node node = get_debug_lines();
    node.set_name("DebugLines");
    
    // Line data
    Nil::Data::Developer line_data;
    line_data.type_id = 2;
    line_data.aux_01  = (uintptr_t)line_info.data();
    line_data.aux_02  = 0;
    
    Nil::Data::set(node, line_data);
  }

  // Mouse
  {
    Nil::Node node = get_mouse();
    node.set_name("Mouse1");
    
    Nil::Data::Mouse ms{};
    ms.id = 0;
    
    Nil::Data::set(node, ms);
  }
  
  // Keyboard
  {
    Nil::Node node = get_keyboard();
    node.set_name("KB1");
    
    Nil::Data::Keyboard kb{};
    kb.id = 0;
    
    Nil::Data::set(node, kb);
  }
  
  // Assets
  {
    ROA::Object node = get_assets();
    node.set_name("Assets");
  }
}


math::vec3
get_world_up()
{
  return math::vec3_init(0,1,0);
}


math::vec3
get_world_left()
{
  return math::vec3_init(1,0,0);
}


math::vec3
get_world_fwd()
{
  return math::vec3_init(0,0,1);
}


Nil::Node
get_app()
{
  static Nil::Node node;
  return node;
}


Nil::Node
get_mouse()
{
  static Nil::Node node;
  return node;
}


Nil::Node
get_keyboard()
{
  static Nil::Node node;
  return node;
}


Nil::Node
get_debug_lines()
{
  static Nil::Node node;
  return node;
}


ROA::Object
get_assets()
{
  static ROA::Object node;
  return node;
}


void
debug_line(const math::vec3 start, const math::vec3 end, const math::vec3 col)
{
  // Line data //
  Nil::Node node = get_debug_lines();
  
  if(node.is_valid() && Nil::Data::has_developer(node))
  {
    Nil::Data::Developer line_data;
    Nil::Data::get(node, line_data);
    
    const size_t count = (size_t)line_data.aux_02;
    
    if(count >= line_info.size())
    {
      line_info.resize(count + 9);
    }
    
    line_info[count + 0] = (math::get_x(start));
    line_info[count + 1] = (math::get_y(start));
    line_info[count + 2] = (math::get_z(start));

    line_info[count + 3] = (math::get_x(end));
    line_info[count + 4] = (math::get_y(end));
    line_info[count + 5] = (math::get_z(end));
    
    line_info[count + 6] = (math::get_x(col));
    line_info[count + 7] = (math::get_y(col));
    line_info[count + 8] = (math::get_z(col));
    
    
    line_data.aux_01 = (uintptr_t)line_info.data();
    line_data.aux_02 += 9;
    
    Nil::Data::set(node, line_data);
  }
}


} // ns
