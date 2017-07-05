#ifndef MATERIAL_INCLUDED_F58F218E_C94E_49D4_9CA3_34CBD7A35E26
#define MATERIAL_INCLUDED_F58F218E_C94E_49D4_9CA3_34CBD7A35E26


#include <stdint.h>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --


struct Material
{
  uint64_t id;
  uint32_t shader_type;
  uint32_t color;
  uint32_t texture_01;
  uint32_t texture_02;
  uint32_t texture_03;
};


// ----------------------------------------------------------------- [ Find ] --


void
find_by_name(const char *name, Material &out);


// ------------------------------------------------------------ [ Set / Get ] --


bool
load(const char *name, Material &in_out);


} // ns
} // ns


#endif // inc guard
