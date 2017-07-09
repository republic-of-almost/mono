#include <imgui/imgui_resource.hpp>
#include <nil/resource/resource.hpp>
#include <math/general/general.hpp>
#include <imgui/imgui.h>
#include <lib/color.hpp>
#include <stdio.h>


namespace Nil {
namespace ImGUI {


// --------------------------------------------------- [ Nil Resource Types ] --


void
render_resource(const Nil::Resource::Texture *rsrc, const size_t count)
{
  ImGui::Text("Texture Count %zu", count);
  
  static uint32_t tex_slider = 6;
  
  char name[32]{};
  sprintf(name, "Size: %d", 1 << tex_slider);
  ImGui::SliderInt("Texture Size", (int*)&tex_slider, 6, 10, name);
  
  ImGui::Separator();
  
  const ImVec2 win_size       = ImGui::GetWindowSize();
  const uint32_t tex_size     = 1 << tex_slider;
  const uint32_t col_size_est = tex_size + 20;
  const uint32_t col_size     = win_size.x / col_size_est;
  const uint32_t cols         = math::max(col_size, uint32_t{1});
  
  for(size_t i = 0; i < count; ++i)
  {
    Nil::Resource::Texture tex = rsrc[i];
  
    ImGui::Image(
      (ImTextureID)tex.platform_resource,
      ImVec2(tex_size, tex_size)
    );
  
    if(ImGui::IsItemHovered())
    {
      ImGui::SetTooltip(
        "ID: %d\nDimentions: %d x %d\nChannels: %d\nResource ID: %p",
        tex.id,
        tex.width,
        tex.height,
        tex.components,
        (void*)tex.platform_resource
      );
    }
    
    if((i + 1) % (cols ))
    {
      ImGui::SameLine();
    }
  }
}


void
render_resource(const Nil::Resource::Material *rsrc, const size_t count)
{
  ImGui::Text("Material Count %zu", count);

  for(size_t i = 0; i < count; ++i)
  {
    const Nil::Resource::Material *data = &rsrc[i];
  
    char name[16]{};
    sprintf(name, "Material#%zu", i + 1);
    
    if(ImGui::CollapsingHeader(name))
    {
      size_t tex_count = 0;
      Nil::Resource::Texture *textures;
      Nil::Resource::get(&tex_count, &textures);
      
      ImGui::Image(
        (void*)textures[data->texture_01].platform_resource,
        ImVec2(64, 64)
      );
      
      ImGui::SameLine();
      
      ImGui::Image(
        (void*)textures[data->texture_02].platform_resource,
        ImVec2(64, 64)
      );
      
      ImGui::SameLine();
      
      ImGui::Image(
        (void*)textures[data->texture_03].platform_resource,
        ImVec2(64, 64)
      );
    
      float color[4]
      {
        lib::color::get_channel_1f(data->color),
        lib::color::get_channel_2f(data->color),
        lib::color::get_channel_3f(data->color),
        lib::color::get_channel_4f(data->color),
      };
    
      ImGui::ColorEdit4("Color##Mat", color);
    }
  }
}


void
render_resource(const Nil::Resource::Mesh *rsrc, const size_t count)
{
  ImGui::Text("Mesh Count %zu", count);

  for(size_t i = 0; i < count; ++i)
  {
    char name[16]{};
    sprintf(name, "Mesh#%zu", i + 1);
  
    if(ImGui::CollapsingHeader(name))
    {
      const Nil::Resource::Mesh *data = &rsrc[i];

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
        ImGui::BeginChild(
          ImGui::GetID((void*)(intptr_t)data->position_vec3),
          ImVec2(ImGui::GetWindowWidth() * col_ratio * 3, 200.0f),
          true
        );
        
        for (int line = 0; line < data->count; line++)
        {
          int index = line * 3;
          char pos[16];
          memset(pos, 0, sizeof(pos));
          sprintf(
            pos,
            "%.2f,%.2f,%.2f",
            data->position_vec3[index + 0],
            data->position_vec3[index + 1],
            data->position_vec3[index + 2]
          );
          ImGui::Text("%s", pos);
        }
        
        ImGui::EndChild();
        ImGui::EndGroup();
        
        ImGui::SameLine();
      }

      // Normals
      if(data->normal_vec3)
      {
        ImGui::BeginGroup();
        ImGui::Text("Normals");
        ImGui::BeginChild(
          ImGui::GetID((void*)data->normal_vec3),
          ImVec2(ImGui::GetWindowWidth() * col_ratio * 3, 200.0f),
          true
        );
        
        for (int line = 0; line < data->count; line++)
        {
          int index = line * 3;
          char pos[16];
          memset(pos, 0, sizeof(pos));
          sprintf(
            pos,
            "%.2f,%.2f,%.2f",
            data->normal_vec3[index + 0],
            data->normal_vec3[index + 1],
            data->normal_vec3[index + 2]
          );
          ImGui::Text("%s", pos);
        }
        
        ImGui::EndChild();
        ImGui::EndGroup();
        
        ImGui::SameLine();
      }

      // UV
      if(data->texture_coords_vec2)
      {
        ImGui::BeginGroup();
        ImGui::Text("UVs");
        ImGui::BeginChild(
          ImGui::GetID((void*)data->texture_coords_vec2),
          ImVec2(ImGui::GetWindowWidth() * col_ratio * 2, 200.0f),
          true
        );
        
        for (int line = 0; line < data->count; line++)
        {
          int index = line * 2;
          char pos[16];
          memset(pos, 0, sizeof(pos));
          sprintf(
            pos,
            "%.2f,%.2f",
            data->texture_coords_vec2[index + 0],
            data->texture_coords_vec2[index + 1]
          );
          ImGui::Text("%s", pos);
        }
        
        ImGui::EndChild();
        ImGui::EndGroup();
        
        ImGui::SameLine();
      }

      // Colors
      if(data->color_vec4)
      {
        ImGui::BeginGroup();
        ImGui::Text("Colors");
        ImGui::BeginChild(
          ImGui::GetID((void*)data->color_vec4),
          ImVec2(ImGui::GetWindowWidth() * col_ratio * 4, 200.0f),
          true
        );
        
        for(int line = 0; line < data->count; line++)
        {
          int index = line * 4;
          char pos[16];
          memset(pos, 0, sizeof(pos));
          sprintf(
            pos,
            "%.2f,%.2f,%.2f,%.2f",
            data->color_vec4[index + 0],
            data->color_vec4[index + 1],
            data->color_vec4[index + 2],
            data->color_vec4[index + 3]
          );
          
          ImGui::Text("%s", pos);
        }
        
        ImGui::EndChild();
        ImGui::EndGroup();
      }
      
      ImGui::Spacing();
    } // if collapsing header
  }
}



} // ns
} // ns
