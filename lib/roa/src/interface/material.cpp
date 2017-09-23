#include <roa/material.hpp>
#include <roa/shader.hpp>
#include <roa/color.hpp>
#include <nil/resource/material.hpp>
#include <lib/logging.hpp>
#include <string.h>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Material::Material()
: Material("")
{
}



Material::Material(const uint32_t id)
: Resource(id)
{
}


Material::Material(const char *name)
: Material(name, Color(0xFFFFFFFF))
{
}


Material::Material(
  const char *name,
  const Color &color
)
: Resource()
{
  if(strlen(name) > 0)
  {
    Nil::Resource::Material mat{};
    mat.name = name;
    mat.color = color.get_uint();
    Nil::Resource::load(mat);
    
    m_id = mat.id;
  }
}


Material::~Material()
{
}


// ------------------------------------------------------------ [ Attibutes ] --


Color
Material::get_color() const
{
  size_t count = 0;
  Nil::Resource::Material *mats = nullptr;
  Nil::Resource::get(&count, &mats);
  
  if(count > m_id)
  {
    return Color(mats[m_id].color);
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
  size_t count = 0;
  Nil::Resource::Material *mats = nullptr;
  Nil::Resource::get(&count, &mats);
  
  if(count > m_id)
  {
    mats[m_id].color = col.get_uint();
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


// ------------------------------------------------------------ [ Inherited ] --


const char *
Material::get_resource_type_name() const
{
}


Resource_status
Material::get_load_status() const
{
}


} // ns
