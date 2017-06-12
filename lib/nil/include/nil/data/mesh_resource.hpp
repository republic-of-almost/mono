/*
  Mesh_resource
  --
  The data will be copied out, so you don't have to keep your copy valid.
*/
#ifndef MESH_RESOURCE_INCLUDED_B4C2715F_0BAC_4042_8E0F_9723D6254925
#define MESH_RESOURCE_INCLUDED_B4C2715F_0BAC_4042_8E0F_9723D6254925


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


struct Mesh_resource
{
  uint32_t id;
  uint32_t status;
  
  float *position_vec3;
  float *normal_vec3;
  float *texture_coords_vec2;
  float *color_vec4;
  
  size_t count;
};


void
get(const Node &node, Mesh_resource &mesh);


void
set(Node &node, const Mesh_resource &mesh);


bool
has_mesh_resource(const Node &node);


uint64_t
get_type_id(const Mesh_resource &mesh);


} // ns
} // ns


#endif // inc guard
