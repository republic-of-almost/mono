#include <nil/data/mesh_resource.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


namespace Nil {
namespace Data {


void
get(const Node &node, Mesh_resource &out)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP

  if(!getter_helper(
        node.get_id(),
        graph->component_data.mesh_resource_node_id,
        graph->component_data.mesh_resource_data,
        out))
  {
    NIL_DATA_GETTER_ERROR(Mesh_resource)
  }
}


void
set(Node &node, const Mesh_resource &in)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP
  
  Mesh_resource cpy_in{};
  cpy_in.count = in.count;
  
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

  if(!setter_helper(
    node,
    graph->component_data.mesh_resource_node_id,
    graph->component_data.mesh_resource_data,
    cpy_in,
    get_type_id(cpy_in)))
  {
    NIL_DATA_SETTER_ERROR(Mesh_resource)
  }
}


bool
has_mesh_resource(const Node &node)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP
  
  return has(node.get_id(), graph->component_data.mesh_resource_node_id);
}


uint64_t
get_type_id(const Mesh_resource &)
{
  NIL_DATA_TYPE_ID_REG
}


} // ns
} // ns
