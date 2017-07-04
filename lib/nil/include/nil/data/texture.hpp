#ifndef TEXTURE_INCLUDED_D5E1A9EE_AE2E_4DFE_941D_6FC13F866E45
#define TEXTURE_INCLUDED_D5E1A9EE_AE2E_4DFE_941D_6FC13F866E45


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Texture
{
  uint32_t id;
  enum { POINT, LINEAR, ANISO_X2, ANISO_X4, ANISO_X8, ANISO_X16 } filtering;
  
  uint32_t width;
  uint32_t height;
  uint32_t depth;
  uint32_t dimentions;
  uint32_t components;
  
  // ** Output ** //
  
  enum { PENDING, LOADED, FAILED, } status;
  uintptr_t platform_id;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Texture **data);


void
get(const Node &node, Texture &out);


void
set(Node &node, const Texture &in);


void
remove_texture(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_texture(const Node &node);


bool
has(const Node &node, const Texture &data);


uint64_t
get_type_id(const Texture &in);


const char*
get_type_name(const Texture &in);


size_t
texture_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Texture **out_data, Node **out_node);


} // ns
} // ns


#endif // inc gaurd
