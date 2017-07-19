#include <roa/mesh.hpp>
#include <roa/bounding_box.hpp>
#include <roa/vector3.hpp>
#include <nil/resource/mesh.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Mesh::Mesh(const char *name)
: Resource()
{
  Nil::Resource::Mesh mesh{};
  Nil::Resource::find_by_name("Unit_bev_cube", mesh);
  
  m_id = mesh.id;
}


// ----------------------------------------------------------- [ Attributes ] --


Bounding_box
Mesh::get_bounding_box() const
{
  return Bounding_box(Vector3(-0.5f, -0.5f, -0.5f), Vector3(0.5f, 0.5f, 0.5f));
}


} // ns
