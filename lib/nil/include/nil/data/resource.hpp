#ifndef RESOURCE_INCLUDED_48E8D0F7_75FD_4EE1_9B6A_959D8ADB9E20
#define RESOURCE_INCLUDED_48E8D0F7_75FD_4EE1_9B6A_959D8ADB9E20


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Resource
{
  enum { UNKNOWN, TEXTURE, MESH, AUDIO } type;
  char filepath[2048];
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Resource **data);


void
get(const Node &node, Resource &out);


void
set(Node &node, const Resource &in);


void
remove_resource(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_resource(const Node &node);


bool
has(const Node &node, const Resource &data);


uint64_t
get_type_id(const Resource &in);


const char*
get_type_name(const Resource &in);


size_t
resource_count();


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Resource **out_data = nullptr, Node **out_node = nullptr);


} // ns
} // ns


#endif // inc gaurd
