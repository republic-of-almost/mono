#ifndef ACTOR_INCLUDED_2F369057_567B_46D2_8C7F_3D3EC28523A9
#define ACTOR_INCLUDED_2F369057_567B_46D2_8C7F_3D3EC28523A9


#include <nil/node.hpp>


namespace Game {


struct Actor
{
  Nil::Node entity;
  Nil::Node head;
  
  float accum_pitch;
  float accum_yaw;
  float height;
  float nav_mesh[18 * 16];
};


void
setup(Actor *actor);


} // ns


#endif // inc guard
