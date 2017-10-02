#ifndef NIMGUI

#ifndef IMGUI_RESOURCE_INCLUDED_8D7DB56E_D91E_41D5_92D9_2250EDE3174E
#define IMGUI_RESOURCE_INCLUDED_8D7DB56E_D91E_41D5_92D9_2250EDE3174E


#include <nil/fwd.hpp>
#include <stddef.h>


namespace Nil {
namespace ImGUI {


// --------------------------------------------------- [ Nil Resource Types ] --


void
render_resource_overview(
  const Nil_texture *tex_rsrc, const size_t tex_count,
  const Nil::Resource::Material *mat_rsrc, const size_t mat_count,
  const Nil_mesh *mesh_rsrc, const size_t mesh_count,
  const Nil_shader *shd_rsrc, const size_t shd_count
 );


void
render_resource(const Nil_texture *rsrc, const size_t count);


void
render_resource(const Nil::Resource::Material *rsrc, const size_t count);


void
render_resource(const Nil_mesh *rsrc, const size_t count);


void
render_resource(const Nil_shader *rsrc, const size_t count);


} // ns
} // ns


#endif // inc guard

#endif // build guard
