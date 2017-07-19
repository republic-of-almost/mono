#include <roa/renderable.hpp>
#include <roa/mesh.hpp>
#include <roa/material.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Renderable::Renderable()
{
}


Renderable::~Renderable()
{
}


// ----------------------------------------------------------- [ Attributes ] --


Mesh
Renderable::get_mesh() const
{
}


void
Renderable::set_mesh(const Mesh &mesh)
{
}


Material
Renderable::get_material() const
{
}


void
Renderable::set_material(const ROA::Material &mat)
{
  
}


} // ns
