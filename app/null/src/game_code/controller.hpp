#ifndef CONTROLLER_INCLUDED_9BF35372_A318_4C43_AC9E_148D6B1370B6
#define CONTROLLER_INCLUDED_9BF35372_A318_4C43_AC9E_148D6B1370B6


#include <roa/roa.hpp>
#include <game_code/fundamental.hpp>


struct Controller
{
  float fwd;
  float left;
  float lookat_axis[2];
  
  // Need to figure out direction.
};


void
controller_update(Controller *controllers, const size_t count);


#endif // inc guard
