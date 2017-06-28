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


// ----------------------------------------------------------------- [ Data ] --


struct Mesh_resource
{
  uint32_t id;
  
  float *position_vec3;
  float *normal_vec3;
  float *texture_coords_vec2;
  float *color_vec4;
  
  size_t count;
  
  // -- Output -- //
  
  enum { PENDING, LOADED, FAILED, } status;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *out_count, Mesh_resource **out_resource);


void
get(const Node &node, Mesh_resource &mesh);


void
set(Node &node, const Mesh_resource &mesh);


void
remove_mesh_resource(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_mesh_resource(const Node &node);


bool
has(const Node &node, const Mesh_resource &data);


uint64_t
get_type_id(const Mesh_resource &mesh);


const char*
get_type_name(const Mesh_resource &in);


size_t
mesh_resource_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Mesh_resource **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
