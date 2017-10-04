#ifndef RESOURCE_IDENTIFIERS_INCLUDED_D6439D8E_717F_4313_B651_D11887E9807D
#define RESOURCE_IDENTIFIERS_INCLUDED_D6439D8E_717F_4313_B651_D11887E9807D


#include <roa/fundamental.hpp>
#include <roa/resource_status.hpp>
#include <roa/shader_type.hpp>
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

static const Nil_resource_status nil_rsrc_status_from_roa_rsrc_status[]
{
  NIL_RSRC_STATUS_NONE,
  NIL_RSRC_STATUS_PENDING,
  NIL_RSRC_STATUS_LOADED,
  NIL_RSRC_STATUS_FAILED,
  NIL_RSRC_STATUS_ERROR,
};


constexpr ROA::Resource_status
convert_from_nil(const Nil_resource_status status)
{
  return (ROA::Resource_status)roa_rsrc_status_from_nil_rsrc_status[(int)status];
}

constexpr Nil_resource_status
convert_from_roa(const ROA::Resource_status status)
{
  return (Nil_resource_status)nil_rsrc_status_from_roa_rsrc_status[(int)status];
}

inline Nil_shader_type
convert_from_roa(const ROA::Shader_type type)
{
  switch(type)
  {
    case(ROA::Shader_type::DEBUG_LINE_RENDERER):
      return NIL_SHD_LINE_RENDERER_01;
    case(ROA::Shader_type::MESH_RENDERER):
      return NIL_SHD_MESH_RENDERER_01;
    case(ROA::Shader_type::PANE_RENDERER):
      return NIL_SHD_PAINT_RENDERER_01;
    case(ROA::Shader_type::NONE):
      return NIL_SHD_NONE;
  }
}


inline ROA::Shader_type
convert_from_nil(const Nil_shader_type type)
{
  switch(type)
  {
    case(NIL_SHD_LINE_RENDERER_01):
      return ROA::Shader_type::DEBUG_LINE_RENDERER;
    case(NIL_SHD_MESH_RENDERER_01):
      return ROA::Shader_type::MESH_RENDERER;
    case(NIL_SHD_PAINT_RENDERER_01):
      return ROA::Shader_type::PANE_RENDERER;
    case(NIL_SHD_NONE):
      return ROA::Shader_type::NONE;
  }
}


} // ns


#endif // inc guard
