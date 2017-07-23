#ifndef ACTOR_INCLUDED_2F369057_567B_46D2_8C7F_3D3EC28523A9
#define ACTOR_INCLUDED_2F369057_567B_46D2_8C7F_3D3EC28523A9


#include <roa/roa.hpp>


namespace Game {


struct Actor
{
  ROA::Object entity;
  ROA::Object head;
  
  float accum_pitch;
  float accum_yaw;
  float height;
  float *nav_mesh;
  uint32_t nav_mesh_count;
};


void
setup(Actor *actor);


} // ns


#endif // inc guard
