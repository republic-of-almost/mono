#ifndef ACTOR_INCLUDED_EDA0C9F4_B9A4_4605_B79A_BA1503E6269F
#define ACTOR_INCLUDED_EDA0C9F4_B9A4_4605_B79A_BA1503E6269F


#include <nil/node.hpp>


namespace Game {


struct Actor
{
  Nil::Node entity;
  Nil::Node head;
  
  float accum_pitch;
  float accum_yaw;
  float height;
  float nav_mesh[18];
};


void
setup(Actor *actor);


} // ns


#endif // inc guard
