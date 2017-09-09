#ifndef TRANSFORM_INCLUDED_77C38CC7_A63D_4206_9184_978F2705C5D9
#define TRANSFORM_INCLUDED_77C38CC7_A63D_4206_9184_978F2705C5D9


#include <nil/fwd.hpp>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Transform
{
  float position[3];
  float scale[3];
  float rotation[4];
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Transform &out, const bool world = false);


void
set(Node &node, const Transform &in);


// ----------------------------------------------------------------- [ Info ] --


bool
has_transform(const Node &node);


bool
has(const Node &node, const Transform &data);


uint64_t
get_type_id(const Transform &in);


const char*
get_type_name(const Transform &in);


size_t
transform_count();


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Transform **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
