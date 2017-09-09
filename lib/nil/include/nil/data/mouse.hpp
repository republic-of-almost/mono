#ifndef MOUSE_INCLUDED_748856B2_7A08_493A_B2EE_D1CFC21A9B6F
#define MOUSE_INCLUDED_748856B2_7A08_493A_B2EE_D1CFC21A9B6F


#include <nil/fwd.hpp>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Mouse
{
  uint32_t id;

  int32_t position[2];
  float delta[2];
  float scroll[2];
  float buttons[5];
  bool capture;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Mouse **data);


void
get(const Node &node, Mouse &out);


void
set(Node &node, const Mouse &in);


void
remove_mouse(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_mouse(const Node &node);


bool
has(const Node &node, const Mouse &data);


uint64_t
get_type_id(const Mouse &in);


const char*
get_type_name(const Mouse &in);


size_t
mouse_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Mouse **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
