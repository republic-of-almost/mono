#ifndef TEXTURE_RESOURCE_INCLUDED_597A17EE_B0A2_4CE7_82DB_44D0DD1FFBAA
#define TEXTURE_RESOURCE_INCLUDED_597A17EE_B0A2_4CE7_82DB_44D0DD1FFBAA


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Texture_resource
{
  uint32_t id;
  uint32_t compoents;
  uint32_t width;
  uint32_t height;
  uint32_t depth;
  uint32_t dimentions;
    
  uint8_t *data;
  size_t sizeof_data;
  
  // -- Output -- //
  
  enum { PENDING, LOADED, FAILED, } status;
  uintptr_t platform_resource;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *out_count, Texture_resource **out_resource);


void
get(const Node &node, Texture_resource &out);


void
set(Node &node, const Texture_resource &in);


void
remove_texture_resource(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_texture_resource(const Node &node);


bool
has(const Node &node, const Texture_resource &data);


uint64_t
get_type_id(const Texture_resource &in);


const char*
get_type_name(const Texture_resource &in);


size_t
texture_resource_count();


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Texture_resource **out_data, Node **out_node);


} // ns
} // ns


#endif // inc gaurd
