#ifndef SHADER_INCLUDED_69FC0F54_CDC9_4734_ACE5_E5369C3A3E57
#define SHADER_INCLUDED_69FC0F54_CDC9_4734_ACE5_E5369C3A3E57


#include <nil/fwd.hpp>


namespace Nil {
namespace Resource {


enum class Shader_type
{
  NONE,
  
  ROV_Dbg_line_renderer,
  ROV_Mesh_renderer,
  
  PANE,
};


struct Shader
{
  // -- Input -- //
  
  Shader_type type;
  
  const char *name;

  const char *vs_code;
  const char *gs_code;
  const char *fs_code;
  
  // -- Output -- //
  
  Load_status status;
  uintptr_t   platform_resource;
  uint32_t    id;
};


// ----------------------------------------------------------------- [ Find ] --


/*!
  Searches for a Shader by name.
  if found returns true else returns false.
*/
bool
find_by_name(const char *name, Shader *out = nullptr);


// ----------------------------------------------------------- [ Get / Load ] --


/*!
  Loads a new Texture.
  Does *not* update an existing texture if name already exists.
  If it fails to load it will return false.
*/
bool
load(Shader &in);


/*!
  Gets access to the underlying data.

  size_t count = 0;
  Texture *data = nullptr;
  get(&count, &data);
*/
void
get(size_t *count, Shader **out);


// ----------------------------------------------------------------- [ Info ] --
/*
  Get general information of textures.
*/


const char *
get_type_name(const Shader &in);


size_t
shader_count();


} // ns
} // ns


#endif // inc guard
