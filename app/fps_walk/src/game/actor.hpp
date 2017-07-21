#ifndef ACTOR_INCLUDED_EDA0C9F4_B9A4_4605_B79A_BA1503E6269F
#define ACTOR_INCLUDED_EDA0C9F4_B9A4_4605_B79A_BA1503E6269F


#include <nil/node.hpp>
#include <roa/roa.hpp>


namespace Game {


struct Actor
{
  ROA::Object entity;
  ROA::Object head;
  
  float accum_pitch  = 1.f;
  float accum_yaw    = 0.f;
  float height       = 2.f;
  float nav_mesh[18];
};


void
setup(Actor *actor);


} // ns


#endif // inc guard
