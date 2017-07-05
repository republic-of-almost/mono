#ifndef MESH_INCLUDED_B8540088_3107_4C5A_A8B8_36D182202C3E
#define MESH_INCLUDED_B8540088_3107_4C5A_A8B8_36D182202C3E


#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --


struct Mesh
{
  uint32_t id;
  
  float   *position_vec3;
  float   *normal_vec3;
  float   *texture_coords_vec2;
  float   *color_vec4;
  
  size_t  count;
  
  // -- Output -- //
  
  enum { PENDING, LOADED, FAILED, } status;

};


// ----------------------------------------------------------------- [ Load ] --


void
load(const char *name, Mesh &in);



} // ns
} // ns


#endif // inc guard
