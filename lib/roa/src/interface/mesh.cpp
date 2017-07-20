#include <roa/mesh.hpp>
#include <roa/bounding_box.hpp>
#include <roa/vector3.hpp>
#include <nil/resource/mesh.hpp>
#include <string.h>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Mesh::Mesh()
: Mesh("")
{
}


Mesh::Mesh(const uint32_t id)
: Resource(id)
{
}


Mesh::Mesh(const char *name)
: Resource()
{
  if(strlen(name) > 0)
  {
    Nil::Resource::Mesh mesh{};
    Nil::Resource::find_by_name("Unit_bev_cube", mesh);
    
    m_id = mesh.id;
  }
}


// ----------------------------------------------------------- [ Attributes ] --


Bounding_box
Mesh::get_bounding_box() const
{
  size_t count = 0;
  Nil::Resource::Mesh *mesh = nullptr;
  Nil::Resource::get(&count, &mesh);
  
  if(count > m_id)
  {
    return Bounding_box(
      Vector3(mesh[m_id].bounding_box.min),
      Vector3(mesh[m_id].bounding_box.max)
    );
  }
  
  return Bounding_box();
}


} // ns
