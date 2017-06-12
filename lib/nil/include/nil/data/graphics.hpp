/*
  Graphics
  --
  This is seperate from window in case you wish the context to create this node.
  that way other aspects can respond to the a delayed creation of the graphics api.
  
  So this data chunk could be seen as a readonly data set.
  
  I would like to make data 'readonly' at some point so that nodes can trigger of
  events better.
*/
#ifndef GRAPHICS_INCLUDED_9B4B722D_167D_4073_BB2A_9EA87C50455D
#define GRAPHICS_INCLUDED_9B4B722D_167D_4073_BB2A_9EA87C50455D


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Graphics
{
  enum { OGL, DX } type;
  uint32_t major;
  uint32_t minor;
  bool active;
};


void
get(const Node &node, Graphics &out);


void
set(Node &node, const Graphics &in);


bool
has_graphics(const Node &node);


uint64_t
get_type_id(const Graphics &in);


} // ns
} // ns


#endif // inc guard
