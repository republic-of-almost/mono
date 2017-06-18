#ifndef RIGIDBODY_INCLUDED_14178F15_7B74_4C66_B874_DBBCE8855BC1
#define RIGIDBODY_INCLUDED_14178F15_7B74_4C66_B874_DBBCE8855BC1


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Rigidbody
{
  float mass;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Rigidbody &out);


void
set(Node &node, const Rigidbody &in);


void
remove_rigidbody(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_rigidbody(const Node &node);


uint64_t
get_type_id(const Rigidbody &in);


size_t
rigidbody_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Rigidbody **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
