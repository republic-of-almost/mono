#ifndef MESH_INCLUDED_B8540088_3107_4C5A_A8B8_36D182202C3E
#define MESH_INCLUDED_B8540088_3107_4C5A_A8B8_36D182202C3E


#include <stdint.h>
#include <stddef.h>
#include <nil/data/bounding_box.hpp>


namespace Nil {
namespace Resource {


// ------------------------------------------------------------- [ Resource ] --


/*!
  The calling code should set the attributes.
  When you successfully load a mesh the id and bounding box will be set.
  When the renderer loads the mesh the platform_resource and status will be set.
*/
struct Mesh
{
  // -- Input -- //

  const char  *name;                // Internally copied
  float       *position_vec3;       // Internally copied
  float       *normal_vec3;         // Internally copied
  float       *texture_coords_vec2; // Internally copied
  float       *color_vec4;          // Internally copied
  
  size_t      triangle_count;
  
  // -- Output -- //
  
  uint32_t id;
  Nil::Data::Bounding_box bounding_box;

  enum { PENDING, LOADED, FAILED, } status;
  uintptr_t platform_resource;
};


// ----------------------------------------------------------------- [ Find ] --


/*!
  Searches for a Mesh by name.
  if found returns true else returns false.
*/
void
find_by_name(const char *name, Mesh &out);


// ----------------------------------------------------------- [ Get / Load ] --


/*!
  Loads a new Mesh.
  Does *not* update an existing mesh if name already exists.
  If it fails to load it will return false.
*/
bool
load(Mesh &in);


/*!
  Gets access to the underlying data.

  size_t count = 0;
  Mesh *data = nullptr;
  get(&count, &data);
*/
void
get(size_t *count, Mesh **out);


// ----------------------------------------------------------------- [ Info ] --


/*!
  Convence method good for templates and UI.
  returns the type name.
*/
const char *
get_type_name(const Mesh &in);


/*!
  Returns the number of meshes in the system.
*/
size_t
mesh_count();


} // ns
} // ns


#endif // inc guard
