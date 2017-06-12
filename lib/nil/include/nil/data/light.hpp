#ifndef LIGHT_INCLUDED_25EFE5DF_D8EC_4D8B_913B_2AAF362CD586
#define LIGHT_INCLUDED_25EFE5DF_D8EC_4D8B_913B_2AAF362CD586


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


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


void
get(const Node &node, Light &out);


void
set(Node &node, const Light &in);


bool
has_light(const Node &node);


uint64_t
get_type_id(const Light &in);


} // ns
} // ns


#endif // inc guard
