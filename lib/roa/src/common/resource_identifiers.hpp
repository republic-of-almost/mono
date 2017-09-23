#ifndef RESOURCE_IDENTIFIERS_INCLUDED_D6439D8E_717F_4313_B651_D11887E9807D
#define RESOURCE_IDENTIFIERS_INCLUDED_D6439D8E_717F_4313_B651_D11887E9807D


#include <roa/fundamental.hpp>


namespace ROA_detail {


namespace Resource {
enum ENUM : uint32_t {

  UNKNOWN = 0,
  MATERIAL,
  SHADER,

};
} // ns


constexpr const char *resource_names[]
{
  "Unknown",
  "Material",
  "Shader"
};


} // ns


#endif // inc guard
