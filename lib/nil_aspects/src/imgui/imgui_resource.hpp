#ifndef IMGUI_RESOURCE_INCLUDED_8D7DB56E_D91E_41D5_92D9_2250EDE3174E
#define IMGUI_RESOURCE_INCLUDED_8D7DB56E_D91E_41D5_92D9_2250EDE3174E


#include <nil/fwd.hpp>
#include <stddef.h>


namespace Nil {
namespace ImGUI {


// --------------------------------------------------- [ Nil Resource Types ] --


void
render_resource(const Nil::Resource::Texture *rsrc, const size_t count);


void
render_resource(const Nil::Resource::Material *rsrc, const size_t count);


void
render_resource(const Nil::Resource::Mesh *rsrc, const size_t count);



} // ns
} // ns


#endif // inc guard
