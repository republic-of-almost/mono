#include <roa/mesh.hpp>
#include <roa/bounding_box.hpp>
#include <roa/vector3.hpp>
#include <common/context.hpp>
#include <nil/resource/mesh.hpp>
#include <lib/logging.hpp>
#include <lib/entity.hpp>
#include <string.h>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}


// ----------------------------------------------------------- [ Attributes ] --


Bounding_box
Mesh::get_bounding_box() const
{
  size_t count = 0;
  Nil_mesh *mesh = nullptr;
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  nil_rsrc_mesh_get_data(ctx, &count, &mesh);
  
  const uint32_t instance = lib::entity::instance(m_id);
  
  if(count > instance)
  {
    return Bounding_box(
      Vector3(mesh[instance].bounding_box.min),
      Vector3(mesh[instance].bounding_box.max)
    );
  }
  
  return Bounding_box();
}


float *
Mesh::get_positions_vec3() const
{
  size_t count = 0;
  Nil_mesh *mesh = nullptr;
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  nil_rsrc_mesh_get_data(ctx, &count, &mesh);
 
  const uint32_t instance = lib::entity::instance(m_id);
 
  if(count > instance)
  {
    return mesh[instance].position_vec3;
  }
  
  return nullptr;
}


float *
Mesh::get_normals_vec3() const
{
  size_t count = 0;
  Nil_mesh *mesh = nullptr;
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  nil_rsrc_mesh_get_data(ctx, &count, &mesh);
  
  const uint32_t instance = lib::entity::instance(m_id);
  
  if(count >= instance)
  {
    return mesh[instance].normal_vec3;
  }
  
  return nullptr;
}


float *
Mesh::get_texture_coords_vec2() const
{
  size_t count = 0;
  Nil_mesh *mesh = nullptr;
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  nil_rsrc_mesh_get_data(ctx, &count, &mesh);
  
  const uint32_t instance = lib::entity::instance(m_id);
  
  if(count > instance)
  {
    return mesh[instance].texture_coords_vec2;
  }
  
  return nullptr;
}


size_t
Mesh::get_triangle_count() const
{
  size_t count = 0;
  Nil_mesh *mesh = nullptr;
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  nil_rsrc_mesh_get_data(ctx, &count, &mesh);
  
  const uint32_t instance = lib::entity::instance(m_id);
  
  if(count > instance)
  {
    return mesh[instance].index_count / 3;
  }
  
  return 0;
}


size_t
Mesh::get_vertex_count() const
{
  size_t count = 0;
  Nil_mesh *mesh = nullptr;
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  nil_rsrc_mesh_get_data(ctx, &count, &mesh);
  
  LOG_TODO("TRIANGLE COUNT IS NOT REPRESENTIVE");
  
  const uint32_t instance = lib::entity::instance(m_id);
  
  if(count > instance)
  {
    return mesh[instance].triangle_count;
  }
  
  return 0;
}


uint32_t*
Mesh::get_index() const
{
  size_t count = 0;
  Nil_mesh *mesh = nullptr;
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  nil_rsrc_mesh_get_data(ctx, &count, &mesh);
  
  const uint32_t instance = lib::entity::instance(m_id);
  
  if(count > instance)
  {
    return mesh[instance].index;
  }
  
  return nullptr;
}


size_t
Mesh::get_index_count() const
{
  size_t count = 0;
  Nil_mesh *mesh = nullptr;
  Nil_ctx *ctx = ROA_detail::get_ctx();
  
  nil_rsrc_mesh_get_data(ctx, &count, &mesh);
  
  const uint32_t instance = lib::entity::instance(m_id);
  
  if(count > instance)
  {
    return mesh[instance].index_count;
  }
  
  return 0;
}


} // ns
