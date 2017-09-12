#ifndef ACTOR_INCLUDED_2F369057_567B_46D2_8C7F_3D3EC28523A9
#define ACTOR_INCLUDED_2F369057_567B_46D2_8C7F_3D3EC28523A9


#include <roa/roa.hpp>


namespace Game {


/*!
  Actor kinematic
  --
  First person character.
  This should split out the controller logic if we ever want AI actors.
*/
struct Actor_kinematic : public ROA::Component
{	
  ROA_COMPONENT_RTTI(Actor_kinematic);
  
  
  // -------------------------------------------------------------- [ Hooks ] --
  
  
  void            on_start() override;
  void            on_think() override;
  
  
  // ------------------------------------------------------------ [ Members ] --
  
  
  ROA::Object     body{nullptr};
  ROA::Object     head{nullptr};
  
  float           accum_pitch{0.f};
  float           accum_yaw{0.f};
  float           height{0.f};
  
  float           *nav_mesh{nullptr};
  uint32_t        nav_mesh_count{0};
};


} // ns


#endif // inc guard
