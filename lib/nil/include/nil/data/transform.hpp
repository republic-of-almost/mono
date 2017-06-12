#ifndef TRANSFORM_INCLUDED_77C38CC7_A63D_4206_9184_978F2705C5D9
#define TRANSFORM_INCLUDED_77C38CC7_A63D_4206_9184_978F2705C5D9


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Transform
{
  float position[3];   // x,y,z
  float scale[3];      // x,y,z
  float rotation[4];   // quaternion x,y,z,w
};


void
get(const Node &node, Transform &out, const bool world = false);


void
set(Node &node, const Transform &in);


bool
has_transform(const Node &node);


uint64_t
get_type_id(const Transform &in);


} // ns
} // ns


#endif // inc guard
