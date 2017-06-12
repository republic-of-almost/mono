#ifndef MOUSE_INCLUDED_748856B2_7A08_493A_B2EE_D1CFC21A9B6F
#define MOUSE_INCLUDED_748856B2_7A08_493A_B2EE_D1CFC21A9B6F


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Mouse
{
  uint32_t id;

  int32_t position[2];
  int32_t delta[2];
  float scroll[2];
  float buttons[5];
  bool capture;
};


void
get(const Node &node, Mouse &out);


void
set(Node &node, const Mouse &in);


bool
has_mouse(const Node &node);


uint64_t
get_type_id(const Mouse &in);



} // ns
} // ns


#endif // inc guard
