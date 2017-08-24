#include "imgui_data.hpp"
#include <nil/data/data.hpp>
#include <stdio.h>


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


void
render_data_overview(
  Nil::Data::Audio *audio_data, const size_t audio_count,
  Nil::Data::Bounding_box *bb_data, const size_t bb_count,
  Nil::Data::Camera *cam_data, const size_t cam_count,
  Nil::Data::Developer *dev_data, const size_t dev_count,
  Nil::Data::Gamepad *gp_data, const size_t gp_count,
  Nil::Data::Keyboard *kb_data, const size_t kb_count,
  Nil::Data::Light *light_data, const size_t light_count,
  Nil::Data::Logic *logic_data, const size_t logic_count,
  Nil::Data::Mouse *ms_data, const size_t ms_count,
  Nil::Data::Renderable *rd_data, const size_t rd_count,
  Nil::Data::Transform *tr_data, const size_t tr_count,
  Nil::Data::Window *wi_data, const size_t wi_count
)
{
  // -- Audio Data Overview -- //
  if(ImGui::CollapsingHeader("Audio##data_over"))
  {
    if(audio_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", audio_count);
    }
    else
    {
      ImGui::Text("No Audio data provided");
    }
  }
  
  // -- Bounding Box Data Overview -- //
  if(ImGui::CollapsingHeader("Bounding Box##data_over"))
  {
    if(bb_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", bb_count);
    }
    else
    {
      ImGui::Text("No Bounding Box data provided");
    }
  }
  
  // -- Camera Data Overview -- //
  if(ImGui::CollapsingHeader("Camera##data_over"))
  {
    if(cam_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", cam_count);
    }
    else
    {
      ImGui::Text("No Camera data provided");
    }
  }
  
  // -- Developer Data Overview -- //
  if(ImGui::CollapsingHeader("Developer##data_over"))
  {
    if(dev_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", dev_count);
    }
    else
    {
      ImGui::Text("No Developer data provided");
    }
  }
  
  // -- Gamepad Data Overview -- //
  if(ImGui::CollapsingHeader("Gamepad##data_over"))
  {
    if(gp_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", gp_count);
    }
    else
    {
      ImGui::Text("No Gamepad data provided");
    }
  }
  
  // -- Keyboard Data Overview -- //
  if(ImGui::CollapsingHeader("Keyboard##data_over"))
  {
    if(kb_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", kb_count);
    }
    else
    {
      ImGui::Text("No Keyboard data provided");
    }
  }
  
  // -- Light Data Overview -- //
  if(ImGui::CollapsingHeader("Light##data_over"))
  {
    if(light_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", light_count);
    }
    else
    {
      ImGui::Text("No Light data provided");
    }
  }
  
  // -- Logic Data Overview -- //
  if(ImGui::CollapsingHeader("Logic##data_over"))
  {
    if(logic_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", logic_count);
    }
    else
    {
      ImGui::Text("No Logic data provided");
    }
  }
  
  // -- Mouse Data Overview -- //
  if(ImGui::CollapsingHeader("Mouse##data_over"))
  {
    if(ms_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", ms_count);
    }
    else
    {
      ImGui::Text("No Mouse data provided");
    }
  }
  
  // -- Renderable Data Overview -- //
  if(ImGui::CollapsingHeader("Renderable##data_over"))
  {
    if(rd_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", rd_count);
    }
    else
    {
      ImGui::Text("No Renderable data provided");
    }
  }
  
  // -- Transform Data Overview -- //
  if(ImGui::CollapsingHeader("Transform##data_over"))
  {
    if(tr_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", tr_count);
    }
    else
    {
      ImGui::Text("No Transform data provided");
    }
  }
  
  // -- Window Data Overview -- //
  if(ImGui::CollapsingHeader("Window##data_over"))
  {
    if(wi_data != nullptr)
    {
      ImGui::LabelText("Count##rover_tex", "%zu", wi_count);
    }
    else
    {
      ImGui::Text("No Window data provided");
    }
  }
}


bool
render_data(Nil::Data::Audio *data)
{
  bool updated = false;
  
  const char *request[] {
    "NONE",
    "PLAY",
    "STOP",
  };
  
  constexpr size_t rq_count = sizeof(request) / sizeof(decltype(request[0]));
  
  const char *state[] {
    "NONE",
    "PLAYING",
    "STOPPED",
  };
  
  constexpr size_t st_count = sizeof(state) / sizeof(decltype(state[0]));
  

  update(ImGui::InputInt("ID##AudioPlayer",   (int*)&data->audio_id),                            &updated);
  update(ImGui::Combo("Request##AudioPlayer", (int*)&data->request_state, request, rq_count),    &updated);
  
  update(ImGui::SliderFloat("Volume##AudioPlayer",  &data->volume, 0.f, 1.f), &updated);
  update(ImGui::Checkbox("Looping##AudioPlayer",    &data->loop), &updated);
  
  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();
  
  constexpr uint32_t flags = ImGuiInputTextFlags_ReadOnly;
  
  ImGui::Text("Readonly Values");

  ImGui::Combo("Current State##AudioPlayer", (int*)&data->current_state, state, st_count, flags);

  return updated;
}


bool
render_data(Nil::Data::Bounding_box *data)
{
  bool updated = false;
  
  update(ImGui::DragFloat3("Min##BB", data->min, 0.1f), &updated);
  update(ImGui::DragFloat3("Max##BB", data->max, 0.1f), &updated);

  return updated;
}


bool
render_data(Nil::Data::Camera *data)
{
  const char *proj[]
  {
    "Perpective",
    "Orthographic",
  };
  
  const size_t count = sizeof(proj) / sizeof(decltype(proj[0]));

  bool updated = false;

  update(ImGui::Combo("Projection##Cam",            (int*)&data->type, proj, count),  &updated);
  update(ImGui::DragInt("Priority##Cam",            (int*)&data->priority),           &updated);
  update(ImGui::DragFloat("Width##Cam",             &data->width, 0.01f),             &updated);
  update(ImGui::DragFloat("Height##Cam",            &data->height, 0.01f),            &updated);
  update(ImGui::DragFloat("FOV##Cam",               &data->fov, 0.01f),               &updated);
  update(ImGui::DragFloat("Near Plane##Cam",        &data->near_plane, 0.1f),         &updated);
  update(ImGui::DragFloat("Far Plane##Cam",         &data->far_plane, 0.1f),          &updated);
  update(ImGui::Checkbox("Clear Color Buffer##Cam", &data->clear_color_buffer),       &updated);
  update(ImGui::Checkbox("Clear Depth Buffer##Cam", &data->clear_depth_buffer),       &updated);
  
  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();
  
  constexpr uint32_t flags = ImGuiInputTextFlags_ReadOnly;
  
  ImGui::Text("Readonly Values");
  
  ImGui::InputFloat3("Position##Cam", data->position);
  ImGui::Spacing();
  ImGui::InputFloat3("ViewMat Row0##Cam", &data->view_mat[0],  flags);
  ImGui::InputFloat3("ViewMat Row1##Cam", &data->view_mat[3],  flags);
  ImGui::InputFloat3("ViewMat Row2##Cam", &data->view_mat[6],  flags);
  ImGui::InputFloat3("ViewMat Row3##Cam", &data->view_mat[12], flags);
  
  return updated;
}


void
render_data(Nil::Data::Camera *cams, const size_t count)
{
  ImGui::Text("Camera Count %zu", count);
  
  ImGui::Columns(8, "camera_list"); // 4-ways, with border
  ImGui::Separator();
  ImGui::Text("Type");        ImGui::NextColumn();
  ImGui::Text("Priority");    ImGui::NextColumn();
  ImGui::Text("Height");      ImGui::NextColumn();
  ImGui::Text("Width");       ImGui::NextColumn();
  ImGui::Text("Near");        ImGui::NextColumn();
  ImGui::Text("Far");         ImGui::NextColumn();
  ImGui::Text("Clr Color");   ImGui::NextColumn();
  ImGui::Text("Clr Depth");   ImGui::NextColumn();
  
  ImGui::Separator();
  
  for(uint32_t i = 0; i < count; ++i)
  {
    ImGui::Text("%s", cams[i].type == Nil::Data::Camera::PERSPECTIVE ? "Pers" : "Ortho"); ImGui::NextColumn();
    ImGui::Text("%d", cams[i].priority);    ImGui::NextColumn();
    ImGui::Text("%f", cams[i].height);      ImGui::NextColumn();
    ImGui::Text("%f", cams[i].width);       ImGui::NextColumn();
    ImGui::Text("%f", cams[i].near_plane);  ImGui::NextColumn();
    ImGui::Text("%f", cams[i].far_plane);   ImGui::NextColumn();
    ImGui::Text("%s", cams[i].clear_color_buffer ? "Yes" : "No"); ImGui::NextColumn();
    ImGui::Text("%s", cams[i].clear_depth_buffer ? "Yes" : "No"); ImGui::NextColumn();
    
    ImGui::Separator();
  }
}


bool
render_data(Nil::Data::Developer *data)
{
  bool updated = false;

  ImGui::Text("Set this in code");

  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();
  
  constexpr uint32_t flags = ImGuiInputTextFlags_ReadOnly;

  ImGui::InputInt("AUX0", (int*)&data->aux_01, flags);
  ImGui::InputInt("AUX1", (int*)&data->aux_02, flags);
  ImGui::InputInt("AUX2", (int*)&data->aux_03, flags);
  ImGui::InputInt("AUX3", (int*)&data->aux_04, flags);

  return updated;
}


bool
render_data(Nil::Data::Light *data)
{
  const char *type []
  {
    "Point",
    "Directional",
  };
  
  const size_t count = sizeof(type) / sizeof(decltype(type[0]));
  
  bool updated = false;
  
  update(ImGui::Combo("Type", (int*)&data->type, type, count), &updated);

  update(ImGui::DragFloat("Atten Const",  &data->atten_const),       &updated);
  update(ImGui::DragFloat("Atten Linear", &data->atten_linear),      &updated);
  update(ImGui::DragFloat("Atten Expon",  &data->atten_exponential), &updated);
  
  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();
  
  constexpr uint32_t flags = ImGuiInputTextFlags_ReadOnly;
  
  ImGui::Text("Readonly Values");

  ImGui::DragFloat3("Position",  data->position,  flags);
  ImGui::DragFloat3("Direction", data->direction, flags);

  return updated;
}


bool
render_data(Nil::Data::Renderable *data)
{
  bool updated = false;

//  const char *shaders[] {
//    "FULLBRIGHT",
//    "LIT",
//    "DIR_LIGHT",
//  };
//  
//  constexpr size_t count = sizeof(shaders) / sizeof(decltype(shaders[0]));

  update(ImGui::InputInt("Mesh", (int*)&data->mesh_id), &updated);
  update(ImGui::InputInt("Material", (int*)&data->material_id), &updated);

  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();
  
  constexpr uint32_t flags = ImGuiInputTextFlags_ReadOnly;
  
  ImGui::Text("Readonly Values");

  ImGui::InputFloat4("MatRow0", (float*)&data->world_mat[0], flags);
  ImGui::InputFloat4("MatRow1", (float*)&data->world_mat[4], flags);
  ImGui::InputFloat4("MatRow2", (float*)&data->world_mat[8], flags);
  ImGui::InputFloat4("MatRow3", (float*)&data->world_mat[12], flags);

  return updated;
}


bool
render_data(Nil::Data::Mouse *data)
{
  bool updated = false;
  
  update(ImGui::Checkbox("Capture", &data->capture), &updated);
  
  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();
  
  constexpr uint32_t flags = ImGuiInputTextFlags_ReadOnly;
  
  ImGui::Text("Readonly Values");
  
  ImGui::InputFloat2("Pos",   (float*)data->position, flags);
  ImGui::InputFloat2("Delta", data->delta,    flags);

  return updated;
}


bool
render_data(Nil::Data::Transform *data)
{
  bool updated = false;

  update(ImGui::DragFloat3("Position##Tra", data->position, 0.1f), &updated);
  update(ImGui::DragFloat3("Scale##Tra",    data->scale, 0.1f),    &updated);
  update(ImGui::DragFloat4("Rotation##Tra", data->rotation, 0.1f), &updated);

  return updated;
}


bool
render_data(Nil::Data::Window *data)
{
  bool updated = false;

  update(ImGui::InputText("Window Title##Win",  data->title, 32),                  &updated);
  update(ImGui::DragInt("Width##Win",           (int*)&data->width, 1, 0, 0xFFFF), &updated);
  update(ImGui::DragInt("Height##Win",          (int*)&data->height,1, 0, 0xFFFF), &updated);
  update(ImGui::Checkbox("Fullscreen##Win",     &data->fullscreen),                &updated);
  
  // -- Readonly -- //
  
  ImGui::Separator();
  ImGui::Spacing();

  constexpr uint32_t flags = ImGuiInputTextFlags_ReadOnly;
  
  ImGui::Text("Readonly Values");
  
  const char *types []
  {
    "None",
    "OpenGL",
  };
  
  constexpr size_t count = sizeof(types) / sizeof(decltype(types[0]));

  ImGui::Combo("API", (int*)&data->type, types, count, flags);
  ImGui::InputInt("Major", (int*)&data->major, flags);
  ImGui::InputInt("Minor", (int*)&data->minor, flags);

  return updated;
}


} // ns
} // ns
