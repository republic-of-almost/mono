#include <roa/renderable.hpp>
#include <roa/mesh.hpp>
#include <roa/material.hpp>
#include <common/common.hpp>
#include <nil/data/renderable.hpp>
#include <nil/node.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Renderable::Renderable()
: Object()
{
  Nil::Node node = ROA_detail::get_node(*this);

  Nil::Data::Renderable renderable{};
  Nil::Data::set(node, renderable);
}


Renderable::Renderable(const ROA_nullptr null)
: Object(null)
{
}


Renderable::Renderable(const Mesh &mesh, const Material &mat)
: Renderable()
{
  set_mesh(mesh);
  set_material(mat);
}


Renderable::~Renderable()
{
}


// ----------------------------------------------------------- [ Attributes ] --


Mesh
Renderable::get_mesh() const
{
  Nil::Data::Renderable data{};
  ROA_detail::get_nil_data(*this, data);
  
//  return Mesh(data.mesh_id);
  return Mesh();
}


void
Renderable::set_mesh(const Mesh &mesh)
{
  Nil::Data::Renderable data{};
  ROA_detail::get_nil_data(*this, data);
  data.mesh_id = mesh.get_id();
  ROA_detail::set_nil_data(*this, data);
}


Material
Renderable::get_material() const
{
  Nil::Data::Renderable data{};
  ROA_detail::get_nil_data(*this, data);
  
  return Material();
}


void
Renderable::set_material(const ROA::Material &mat)
{
  Nil::Data::Renderable data{};
  ROA_detail::get_nil_data(*this, data);
  data.material_id = mat.get_id();
  ROA_detail::set_nil_data(*this, data);
}


} // ns
