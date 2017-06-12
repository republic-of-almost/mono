#ifndef DATA_INCLUDED_
#define DATA_INCLUDED_


#include <nil/node.hpp>
#include <math/vec/vec_fwd.hpp>


enum class Game_asset {

  CUBE = 1,
  CUBE_BEV,

}; // enum


namespace Game_data {


void
load_assets();


void
setup();


math::vec3
get_world_up();


math::vec3
get_world_left();


math::vec3
get_world_fwd();


Nil::Node
get_app();


Nil::Node
get_mouse();


Nil::Node
get_keyboard();


Nil::Node
get_debug_lines();


Nil::Node
get_assets();


void
debug_line(const math::vec3 start, const math::vec3 end, const math::vec3 col);

} // ns


#endif // inc guard
