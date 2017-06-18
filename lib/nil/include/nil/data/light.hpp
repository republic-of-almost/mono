#ifndef LIGHT_INCLUDED_25EFE5DF_D8EC_4D8B_913B_2AAF362CD586
#define LIGHT_INCLUDED_25EFE5DF_D8EC_4D8B_913B_2AAF362CD586


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


namespace Light_type {
enum ENUM {
  
  DIRECTIONAL,
  POINT,
  SPOT,
  
  COUNT,

};
} // ns


struct Light
{
  uint32_t type;
  uint8_t color[3];
  float attenuation;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Light &out);


void
set(Node &node, const Light &in);


void
remove_light(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_light(const Node &node);


uint64_t
get_type_id(const Light &in);


size_t
light_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Light **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
