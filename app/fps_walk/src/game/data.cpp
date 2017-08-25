#include <game/data.hpp>
#include <nil/data/window.hpp>
#include <nil/data/mouse.hpp>
#include <nil/data/keyboard.hpp>
#include <math/vec/vec3.hpp>
#include <lib/string.hpp>

namespace Game_data {



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


} // ns
