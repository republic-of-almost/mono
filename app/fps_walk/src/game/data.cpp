#include <game/data.hpp>
#include <nil/data/window.hpp>
#include <nil/data/mouse.hpp>
#include <nil/data/keyboard.hpp>
#include <math/vec/vec3.hpp>
#include <lib/string.hpp>

namespace Game_data {


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


} // ns
