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
render_data(Nil::Data::Mesh *data)
{
  bool updated = false;

  update(ImGui::InputInt("Mesh ID",  (int*)&data->mesh_id),  &updated);
  update(ImGui::InputInt("Index ID", (int*)&data->index_id), &updated);

  return updated;
}


bool
render_data(Nil::Data::Mesh_resource *data)
{
  bool updated = false;

  update(ImGui::InputInt("Mesh_id", (int*)&data->id), &updated);


  float columns = 3; // put position as default.

  if(data->normal_vec3)         { columns += 3.f; }
  if(data->texture_coords_vec2) { columns += 2.f; }
  if(data->color_vec4)          { columns += 4.f; }

  float col_ratio = (1.f / columns) * 0.9f;

  ImGui::Text("Vertex Count: %zu", data->count);

  // Positions
  if(data->position_vec3)
  {
    ImGui::BeginGroup();
    ImGui::Text("Positions");
    ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)0), ImVec2(ImGui::GetWindowWidth() * col_ratio * 3, 200.0f), true);
    for (int line = 0; line < data->count; line++)
    {
      int index = line * 3;
      char pos[16];
      memset(pos, 0, sizeof(pos));
      sprintf(pos, "%.2f,%.2f,%.2f", data->position_vec3[index + 0], data->position_vec3[index + 1], data->position_vec3[index + 2]);
      ImGui::Text("%s", pos);
    }
    ImGui::EndChild();
    ImGui::EndGroup();
  }

  ImGui::SameLine();

  // Normals
  if(data->normal_vec3)
  {
    ImGui::BeginGroup();
    ImGui::Text("Normals");
    ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)1), ImVec2(ImGui::GetWindowWidth() * col_ratio * 3, 200.0f), true);
    for (int line = 0; line < data->count; line++)
    {
      int index = line * 3;
      char pos[16];
      memset(pos, 0, sizeof(pos));
      sprintf(pos, "%.2f,%.2f,%.2f", data->normal_vec3[index + 0], data->normal_vec3[index + 1], data->normal_vec3[index + 2]);
      ImGui::Text("%s", pos);
    }
    ImGui::EndChild();
    ImGui::EndGroup();
  }

  ImGui::SameLine();

  // UV
  if(data->texture_coords_vec2)
  {
    ImGui::BeginGroup();
    ImGui::Text("UVs");
    ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)2), ImVec2(ImGui::GetWindowWidth() * col_ratio * 2, 200.0f), true);
    for (int line = 0; line < data->count; line++)
    {
      int index = line * 2;
      char pos[16];
      memset(pos, 0, sizeof(pos));
      sprintf(pos, "%.2f,%.2f", data->texture_coords_vec2[index + 0], data->texture_coords_vec2[index + 1]);
      ImGui::Text("%s", pos);
    }
    ImGui::EndChild();
    ImGui::EndGroup();
  }

  ImGui::SameLine();

  // Colors
  if(data->color_vec4)
  {
    ImGui::BeginGroup();
    ImGui::Text("Colors");
    ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)3), ImVec2(ImGui::GetWindowWidth() * col_ratio * 4, 200.0f), true);
    for (int line = 0; line < data->count; line++)
    {
      int index = line * 4;
      char pos[16];
      memset(pos, 0, sizeof(pos));
      sprintf(pos, "%.2f,%.2f,%.2f,%.2f", data->color_vec4[index + 0], data->color_vec4[index + 1], data->color_vec4[index + 2], data->color_vec4[index + 3]);
      ImGui::Text("%s", pos);
    }
    ImGui::EndChild();
    ImGui::EndGroup();
    ImGui::EndChild();
    ImGui::EndGroup();
  }

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
  
  ImGui::InputInt2("Pos",   (int*)data->position, flags);
  ImGui::InputInt2("Delta", (int*)data->delta,    flags);

  return updated;
}


bool
render_data(Nil::Data::Resource *data)
{
  bool updated = false;
  
//  ImGui::Text("Set in code.");
//
//  // -- Readonly -- //
//  
//  ImGui::Separator();
//  ImGui::Spacing();
//
//  constexpr uint32_t flags = ImGuiInputTextFlags_ReadOnly;
//
//  ImGui::Text("Readonly Values");
//
//  ImGui::InputInt("Type",   (int*)&data->type, 0, 0,  flags);
//  ImGui::InputText("Name",  &data->name[0], 64,       flags);
//  ImGui::InputInt("Ptr",    (int*)&data->data, 0, 0,  flags);
  
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
