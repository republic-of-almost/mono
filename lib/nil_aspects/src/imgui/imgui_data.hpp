#ifndef IMGUI_DATA_INCLUDED_42CB464A_F95B_4BB7_A948_183F342A2F3C
#define IMGUI_DATA_INCLUDED_42CB464A_F95B_4BB7_A948_183F342A2F3C


#include <nil/fwd.hpp>
#include <imgui/imgui.h>


namespace Nil {
namespace ImGUI {


// ------------------------------------------------------- [ Nil Data Types ] --

bool
render_data(Nil::Data::Camera *data);


bool
render_data(Nil::Data::Light *data);


/*
  Fallback
*/
template<typename T>
bool
render_data(T *data)
{
  ImGui::Text("No UI Impl");
  return false;
}


} // ns
} // ns


#endif // inc guard
