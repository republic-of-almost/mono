#ifndef MATERIAL_INCLUDED_F58F218E_C94E_49D4_9CA3_34CBD7A35E26
#define MATERIAL_INCLUDED_F58F218E_C94E_49D4_9CA3_34CBD7A35E26


#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --


struct Material
{
  uint32_t color;
  uint32_t texture_01;
  uint32_t texture_02;
  uint32_t texture_03;
  
  // -- Output -- //
  
  uint32_t id;
};


// ----------------------------------------------------------------- [ Find ] --


bool
find_by_name(const char *name, Material &out);


// ------------------------------------------------------------ [ Set / Get ] --


bool
load(const char *name, Material &in_out);


void
get(size_t *count, Material **out);


} // ns
} // ns


#endif // inc guard
