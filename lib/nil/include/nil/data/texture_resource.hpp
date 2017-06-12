#ifndef TEXTURE_RESOURCE_INCLUDED_597A17EE_B0A2_4CE7_82DB_44D0DD1FFBAA
#define TEXTURE_RESOURCE_INCLUDED_597A17EE_B0A2_4CE7_82DB_44D0DD1FFBAA


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


struct Texture_resource
{
  uint32_t id;
  uint32_t compoents;
  uint32_t dimentions;
    
  uint8_t *data;
  size_t sizeof_data;
};


void
get(const Node &node, Texture_resource &out);


void
set(Node &node, const Texture_resource &in);


bool
has_texture_resource(const Node &node);


uint64_t
get_type_id(const Texture_resource &in);


} // ns
} // ns


#endif // inc gaurd
