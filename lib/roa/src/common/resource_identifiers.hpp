#ifndef RESOURCE_IDENTIFIERS_INCLUDED_D6439D8E_717F_4313_B651_D11887E9807D
#define RESOURCE_IDENTIFIERS_INCLUDED_D6439D8E_717F_4313_B651_D11887E9807D


#include <roa/fundamental.hpp>
#include <roa/resource_status.hpp>
#include <nil/fwd.hpp>


namespace ROA_detail {


namespace Resource {
enum ENUM : uint32_t {

  UNKNOWN = 0,
  
  AUDIO,
  FONT,
  MATERIAL,
  MESH,
  MODEL,
  SHADER,
  TEXTURE,
};
} // ns


/*
  Order must be the same as Resource::ENUM
*/
constexpr const char *resource_names[]
{
  "Unknown",
  
  "Audio",
  "Font",
  "Material",
  "Mesh",
  "Model",
  "Shader",
  "Texture",
};

static const char *
convert_from_type_id(const uint32_t type_id)
{
  return resource_names[type_id];
}


/*
  Converts Nil::Resource::Load_status to ROA::Resource_status
  Be carful of order matching.
*/
static const ROA::Resource_status roa_rsrc_status_from_nil_rsrc_status[]
{
  ROA::Resource_status::WAITING,  // Nil::Rsource::Load_status::NONE
  ROA::Resource_status::PENDING,  // Nil::Rsource::Load_status::PENDING
  ROA::Resource_status::LOADED,   // Nil::Rsource::Load_status::LOADED
  ROA::Resource_status::FAILED,   // Nil::Rsource::Load_status::FAILED
  ROA::Resource_status::UNKNOWN,  // Nil::Rsource::Load_status::ERROR
};


constexpr ROA::Resource_status
convert_from_nil(const Nil::Resource::Load_status status)
{
  return (ROA::Resource_status)roa_rsrc_status_from_nil_rsrc_status[(int)status];
}


} // ns


#endif // inc guard
