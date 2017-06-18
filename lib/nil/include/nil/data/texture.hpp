#ifndef TEXTURE_INCLUDED_D5E1A9EE_AE2E_4DFE_941D_6FC13F866E45
#define TEXTURE_INCLUDED_D5E1A9EE_AE2E_4DFE_941D_6FC13F866E45


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


namespace Texture_filtering {
enum ENUM {

  POINT,
  LINEAR,
  ANISO_X2,
  ANISO_X4,
  ANISO_X8,
  ANISO_X16,

}; // enum
} // ns


struct Texture
{
  uint32_t id;
  uint32_t filtering;
  uintptr_t resource;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Texture &out);


void
set(Node &node, const Texture &in);


void
remove_texture(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_texture(const Node &node);


uint64_t
get_type_id(const Texture &in);


size_t
texture_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Texture **out_data, Node **out_node);


} // ns
} // ns


#endif // inc gaurd
