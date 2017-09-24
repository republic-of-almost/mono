#include <roa/material.hpp>
#include <roa/shader.hpp>
#include <roa/color.hpp>
#include <common/resource_identifiers.hpp>
#include <nil/resource/material.hpp>
#include <lib/logging.hpp>
#include <lib/entity.hpp>
#include <string.h>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Material::Material()
: Resource()
{
}


Material::~Material()
{
}


// ------------------------------------------------------------ [ Attibutes ] --


Color
Material::get_color() const
{
  const uint32_t instance = lib::entity::instance(m_id);

  size_t count = 0;
  Nil::Resource::Material *mats = nullptr;
  Nil::Resource::get(&count, &mats);
  
  if(count > instance)
  {
    return Color(mats[instance].color);
  }
  else
  {
    LOG_ERROR("Failed to find material");
    return Color();
  }
}


void
Material::set_color(const Color &col)
{
  const uint32_t instance = lib::entity::instance(m_id);

  size_t count = 0;
  Nil::Resource::Material *mats = nullptr;
  Nil::Resource::get(&count, &mats);
  
  if(count > instance)
  {
    mats[instance].color = col.get_uint();
  }
  else
  {
    LOG_ERROR("Failed to find material");
  }
}


Shader
Material::get_shader() const
{
  return Shader{};
}


void
Material::set_shader(const Shader &shader)
{
}


} // ns
