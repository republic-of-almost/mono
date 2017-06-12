#include <data/data.hpp>
#include <nil/data/window.hpp>
#include <nil/data/developer.hpp>
#include <nil/data/mouse.hpp>
#include <nil/data/keyboard.hpp>
#include <math/vec/vec3.hpp>
#include <lib/array.hpp>


namespace Game_data {


namespace
{
  lib::array<float> line_info;
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
    line_data.aux_01 = (uintptr_t)line_info.data();
    line_data.aux_02 = 0;
    
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


void
debug_line(const math::vec3 start, const math::vec3 end, const math::vec3 col)
{
  // Line data //
  Nil::Node node = get_debug_lines();
  
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


} // ns
