#include <roa/mesh.hpp>
#include <roa/bounding_box.hpp>
#include <roa/vector3.hpp>
#include <nil/resource/mesh.hpp>
#include <lib/logging.hpp>
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
    Nil::Resource::find_by_name(name, mesh);
    
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


float *
Mesh::get_positions_vec3() const
{
  size_t count = 0;
  Nil::Resource::Mesh *mesh = nullptr;
  Nil::Resource::get(&count, &mesh);
 
  if(count > m_id)
  {
    return mesh[m_id].position_vec3;
  }
  
  return nullptr;
}


float *
Mesh::get_normals_vec3() const
{
  size_t count = 0;
  Nil::Resource::Mesh *mesh = nullptr;
  Nil::Resource::get(&count, &mesh);
  
  if(count >= m_id)
  {
    return mesh[m_id].normal_vec3;
  }
  
  return nullptr;
}


float *
Mesh::get_texture_coords_vec2() const
{
  size_t count = 0;
  Nil::Resource::Mesh *mesh = nullptr;
  Nil::Resource::get(&count, &mesh);
  
  if(count > m_id)
  {
    return mesh[m_id].texture_coords_vec2;
  }
  
  return nullptr;
}


size_t
Mesh::get_triangle_count() const
{
  size_t count = 0;
  Nil::Resource::Mesh *mesh = nullptr;
  Nil::Resource::get(&count, &mesh);
  
  if(count > m_id)
  {
    return mesh[m_id].index_count / 3;
  }
  
  return 0;
}


size_t
Mesh::get_vertex_count() const
{
  size_t count = 0;
  Nil::Resource::Mesh *mesh = nullptr;
  Nil::Resource::get(&count, &mesh);
  
  LOG_TODO("TRIANGLE COUNT IS NOT REPRESENTIVE");
  
  if(count > m_id)
  {
    return mesh[m_id].triangle_count;
  }
  
  return 0;
}


uint32_t*
Mesh::get_index() const
{
  size_t count = 0;
  Nil::Resource::Mesh *mesh = nullptr;
  Nil::Resource::get(&count, &mesh);
  
  if(count > m_id)
  {
    return mesh[m_id].index;
  }
  
  return nullptr;
}


size_t
Mesh::get_index_count() const
{
  size_t count = 0;
  Nil::Resource::Mesh *mesh = nullptr;
  Nil::Resource::get(&count, &mesh);
  
  if(count > m_id)
  {
    return mesh[m_id].index_count;
  }
  
  return 0;
}


} // ns
