/*
  Actor
  --
  This is the player object.
*/
#ifndef ACTOR_INCLUDED_2F369057_567B_46D2_8C7F_3D3EC28523A9
#define ACTOR_INCLUDED_2F369057_567B_46D2_8C7F_3D3EC28523A9


#include <roa/roa.hpp>


namespace Game {


struct Actor : ROA::Component
{
  ROA_COMPONENT_RTTI(Actor);
  
  
  // -------------------------------------------------------------- [ Hooks ] --
  
  
  void on_start() override;
  void on_think() override;
  
  
  // ------------------------------------------------------------ [ Members ] --
  
  
  ROA::Object head;
  
  float accum_pitch = 0.f;
  float accum_yaw = 0.f;
  float height = 0.f;
  float *nav_mesh = nullptr;
  uint32_t nav_mesh_count = 0;
};


} // ns


#endif // inc guard
