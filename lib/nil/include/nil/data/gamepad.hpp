/*
  Gamepad
  --
  Gamepad should have enough mappings for most gamepad types, its quite big already.
  The buttons are float if they need to contain pressure information. Unfortunatly 
  this makes it quite a big type, so I may trim it down later.s
*/
#ifndef GAMEPAD_INCLUDED_A73A99A4_56E5_426E_9ECF_CDDBF046FCF1
#define GAMEPAD_INCLUDED_A73A99A4_56E5_426E_9ECF_CDDBF046FCF1


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Gamepad
{
  uint32_t id;

  float stick_0[2];
  float stick_1[2];
  
  float buttons[16];
  float triggers[4];
};


void
get(const Node &node, Gamepad &out);


void
set(Node &node, const Gamepad &in);


bool
has_gamepad(const Node &node);


uint64_t
get_type_id(const Gamepad &in);


} // ns
} // ns


#endif // inc guard
