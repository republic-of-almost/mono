#include "imgui_data.hpp"
#include <nil/data/data.hpp>


namespace Nil {
namespace ImGUI {


// ------------------------------------------------------- [ Nil Data Types ] --


namespace {


inline void
update(bool updated, bool *out_updated)
{
  if(updated)
  {
    *out_updated = true;
  }
}


} // anon ns


bool
render_data(Nil::Data::Camera *data)
{
  const char *proj[]
  {
    "Perpective",
    "Orthographic",
  };

  bool updated = false;

  update(ImGui::Combo("Projection##Cam",            (int*)&data->type, proj, 2), &updated);
  update(ImGui::DragInt("Priority##Cam",            (int*)&data->priority),      &updated);
  update(ImGui::DragFloat("Width##Cam",             &data->width, 0.01f),        &updated);
  update(ImGui::DragFloat("Height##Cam",            &data->height, 0.01f),       &updated);
  update(ImGui::DragFloat("FOV##Cam",               &data->fov, 0.01f),          &updated);
  update(ImGui::DragFloat("Near Plane##Cam",        &data->near_plane, 0.1f),    &updated);
  update(ImGui::DragFloat("Far Plane##Cam",         &data->far_plane, 0.1f),     &updated);
  update(ImGui::Checkbox("Clear Color Buffer##Cam", &data->clear_color_buffer),  &updated);
  update(ImGui::Checkbox("Clear Depth Buffer##Cam", &data->clear_depth_buffer),  &updated);
  
  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();
  
  const uint32_t flags = ImGuiInputTextFlags_ReadOnly;
  
  ImGui::Text("Readonly Values");
  
  ImGui::InputFloat3("Position##Cam", data->position);
  ImGui::Spacing();
  ImGui::InputFloat3("ViewMat Row0##Cam", &data->view_mat[0],  flags);
  ImGui::InputFloat3("ViewMat Row1##Cam", &data->view_mat[3],  flags);
  ImGui::InputFloat3("ViewMat Row2##Cam", &data->view_mat[6],  flags);
  ImGui::InputFloat3("ViewMat Row3##Cam", &data->view_mat[12], flags);
  
  return updated;
}


bool
render_data(Nil::Data::Light *light)
{
  const char *type []
  {
    "Point",
    "Directional",
  };
  
  bool updated = false;

  update(ImGui::Combo("Type", (int*)&light->type, type, 2), &updated);

  update(ImGui::DragFloat("Atten Const",       &light->atten_const),       &updated);
  update(ImGui::DragFloat("Atten Linear",      &light->atten_linear),      &updated);
  update(ImGui::DragFloat("Atten Exponential", &light->atten_exponential), &updated);

  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();
  
  const uint32_t flags = ImGuiInputTextFlags_ReadOnly;
  
  ImGui::Text("Readonly Values");

  ImGui::DragFloat3("Position",  light->position,  flags);
  ImGui::DragFloat3("Direction", light->direction, flags);

  return updated;
}


} // ns
} // ns
