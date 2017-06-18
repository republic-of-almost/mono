#ifndef RESOURCE_INCLUDED_48E8D0F7_75FD_4EE1_9B6A_959D8ADB9E20
#define RESOURCE_INCLUDED_48E8D0F7_75FD_4EE1_9B6A_959D8ADB9E20


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


namespace Resource_type {
enum ENUM {
  DATA,
  SHADER,
  TEXTURE,
  MESH,
  
  COUNT,
};
} // ns


struct Resource
{
  uint32_t type;
  char name[64];
  
  uintptr_t data;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Resource &out);


void
set(Node &node, const Resource &in);


void
remove_resource(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_resource(const Node &node);


uint64_t
get_type_id(const Resource &in);


size_t
resource_count();


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Resource **out_data = nullptr, Node **out_node = nullptr);


} // ns
} // ns


#endif // inc gaurd
