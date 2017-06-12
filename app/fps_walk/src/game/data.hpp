#ifndef DATA_INCLUDED_AAE60048_D437_4386_97E4_0E5C1C0A933C
#define DATA_INCLUDED_AAE60048_D437_4386_97E4_0E5C1C0A933C


#include <nil/node.hpp>
#include <math/vec/vec_fwd.hpp>


namespace Game_data {


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




} // ns


#endif // inc guard
