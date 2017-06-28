#include <nil/data/mesh_resource.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"

// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Mesh_resource>&
get_mesh_rsrc_data()
{
  static Nil::Data::Generic_data<Nil::Data::Mesh_resource> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *out_count, Mesh_resource **out_resource)
{
  *out_count    = get_mesh_rsrc_data().data.size();
  *out_resource = get_mesh_rsrc_data().data.data();
}


void
get(const Node &node, Mesh_resource &out)
{
  get_mesh_rsrc_data().get_data(node, out);
}


void
set(Node &node, const Mesh_resource &in)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP
  
  Mesh_resource cpy_in{};
  cpy_in.count = in.count;
  cpy_in.id    = in.id;
  
  if(in.status > 0)
  {
    cpy_in.status = in.status;
    
    // Copy all the mesh data.
    {
      if(in.position_vec3)
      {
        const size_t data_size = sizeof(float) * in.count * 3;
        float *cpy_data = (float*)malloc(data_size);
        memcpy(cpy_data, in.position_vec3, data_size);
        
        cpy_in.position_vec3 = cpy_data;
      }
      
      if(in.normal_vec3)
      {
        const size_t data_size = sizeof(float) * in.count * 3;
        float *cpy_data = (float*)malloc(data_size);
        memcpy(cpy_data, in.normal_vec3, data_size);
        
        cpy_in.normal_vec3 = cpy_data;
      }
      
      if(in.texture_coords_vec2)
      {
        const size_t data_size = sizeof(float) * in.count * 2;
        float *cpy_data = (float*)malloc(data_size);
        memcpy(cpy_data, in.texture_coords_vec2, data_size);
        
        cpy_in.texture_coords_vec2 = cpy_data;
      };
      
      if(in.color_vec4)
      {
        const size_t data_size = sizeof(float) * in.count * 4;
        float *cpy_data = (float*)malloc(data_size);
        memcpy(cpy_data, in.color_vec4, data_size);
        
        cpy_in.color_vec4 = cpy_data;
      };
    }
  }
  else
  {
    cpy_in.position_vec3 = in.position_vec3;
    cpy_in.normal_vec3 = in.normal_vec3;
    cpy_in.texture_coords_vec2 = in.texture_coords_vec2;
  }

  get_mesh_rsrc_data().set_data(node, cpy_in);
}


void
remove_mesh_resource(Node &node)
{
  get_mesh_rsrc_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_mesh_resource(const Node &node)
{
  return get_mesh_rsrc_data().find(node);
}


bool
has(const Node &node, const Mesh_resource &)
{
  return has_mesh_resource(node);
}


uint64_t
get_type_id(const Mesh_resource &)
{
  return get_mesh_rsrc_data().type_id;
}


const char*
get_type_name(const Mesh_resource &in)
{
  return "Mesh_resource";
}


size_t
mesh_resource_count()
{
  return get_mesh_rsrc_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Mesh_resource **out_data, Node **out_node)
{
  return get_mesh_rsrc_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
