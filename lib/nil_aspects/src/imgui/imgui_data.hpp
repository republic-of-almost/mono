#ifndef NIMGUI

#ifndef IMGUI_DATA_INCLUDED_42CB464A_F95B_4BB7_A948_183F342A2F3C
#define IMGUI_DATA_INCLUDED_42CB464A_F95B_4BB7_A948_183F342A2F3C


#include <nil/fwd.hpp>
#include <imgui/imgui.h>


namespace Nil {
namespace ImGUI {


// ------------------------------------------------------- [ Nil Data Types ] --


void
render_data_overview(
  Nil::Data::Audio *audio_data, const size_t audio_count,
  Nil::Data::Bounding_box *bb_data, const size_t bb_count,
  Nil::Data::Camera *cam_data, const size_t cam_count,
  Nil::Data::Developer *dev_data, const size_t dev_count,
  Nil::Data::Gamepad *gp_data, const size_t cp_count,
  Nil::Data::Keyboard *kb_data, const size_t kb_count,
  Nil::Data::Light *light_data, const size_t light_count,
  Nil::Data::Logic *logic_data, const size_t logic_count,
  Nil::Data::Mouse *ms_data, const size_t ms_count,
  Nil::Data::Renderable *rd_data, const size_t rd_count,
  Nil::Data::Transform *tr_data, const size_t tr_count,
  Nil::Data::Window *wi_data, const size_t wi_count
);


bool
render_data(Nil::Data::Audio *data);


bool
render_data(Nil::Data::Bounding_box *data);


bool
render_data(Nil::Data::Camera *data);


void
render_data(Nil::Data::Camera *data, const size_t count);


bool
render_data(Nil::Data::Developer *data);


bool
render_data(Nil::Data::Light *data);


bool
render_data(Nil::Data::Renderable *data);


bool
render_data(Nil::Data::Mouse *data);


bool
render_data(Nil::Data::Transform *data);


bool
render_data(Nil::Data::Window *data);


/*
  No renderer found
*/
template<typename T>
bool
render_data(T *data)
{
  ImGui::Text("No UI Impl");
  return false;
}


template<typename T>
bool
render_data(T *data, const size_t count)
{
  ImGui::Text("No UI Impl");
  return false;
}


} // ns
} // ns


#endif // inc guard

#endif // build guard
