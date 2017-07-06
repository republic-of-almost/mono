#ifndef TEXTURE_INCLUDED_56366719_6675_4A06_AEA8_5615D7AA8B07
#define TEXTURE_INCLUDED_56366719_6675_4A06_AEA8_5615D7AA8B07


#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --


struct Texture
{
  enum { POINT, LINEAR, ANISO_X2, ANISO_X4, ANISO_X8, ANISO_X16 } filter;
  
  // ** Input ** //
  
  enum { FILENAME } data_type;
  
  uintptr_t data;
  size_t data_size;
  
  // ** Input / Output ** //
  
  uint32_t width;
  uint32_t height;
  uint32_t depth;
  uint32_t components;
  
  // ** Output ** //
  
  enum { PENDING, LOADED, FAILED, } status;
  uintptr_t platform_resource;
  
  uint32_t id;
};


// ----------------------------------------------------------- [ Get / Load ] --


void
load(const char *name, Texture &in);


void
get(size_t *count, Texture **in_out);


} // ns
} // ns


#endif // inc guard
