#ifndef RIGIDBODY_INCLUDED_14178F15_7B74_4C66_B874_DBBCE8855BC1
#define RIGIDBODY_INCLUDED_14178F15_7B74_4C66_B874_DBBCE8855BC1


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Rigidbody
{
  float mass;
};


void
get(const Node &node, Rigidbody &out);


void
set(Node &node, const Rigidbody &in);


bool
has_rigidbody(const Node &node);


uint64_t
get_type_id(const Rigidbody &in);


} // ns
} // ns


#endif // inc guard
