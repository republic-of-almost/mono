#include <roa/material.hpp>
#include <roa/color.hpp>
#include <nil/resource/material.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Material::Material(const char *name)
: Resource()
{
  Nil::Resource::Material mat{};
  mat.color = 0xFF0000FF;
  Nil::Resource::load(name, mat);
  
  m_id = mat.id;
}


Material::~Material()
{
}


// ------------------------------------------------------------ [ Attibutes ] --


Color
Material::get_color() const
{
}


void
Material::set_color(const Color &col)
{
}



} // ns
