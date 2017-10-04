#ifndef NIMGUI

#include <imgui/imgui_resource.hpp>
#include <nil/resource/resource.hpp>
#include <math/general/general.hpp>
#include <imgui/imgui.h>
#include <lib/color.hpp>
#include <lib/helpers.hpp>
#include <lib/assert.hpp>
#include <stdio.h>


namespace Nil {
namespace ImGUI {


// --------------------------------------------------- [ Nil Resource Types ] --


void render_resource_overview(
  const Nil_texture *tex_rsrc, const size_t tex_count,
  const Nil_material *mat_rsrc, const size_t mat_count,
  const Nil_mesh *mesh_rsrc, const size_t mesh_count,
  const Nil_shader *shd_rsrc, const size_t shd_count)
{
  // -- Texture Resource Overview -- //
  if (ImGui::CollapsingHeader("Textures##rsrc_over"))
  {
    if(tex_rsrc != nullptr)
    {
      size_t load_status[4]{};

      // Gather stats //
      for (size_t i = 0; i < tex_count; ++i)
      {
        const Nil_texture *data = &tex_rsrc[i];
        LIB_ASSERT((int)data->status < LIB_ARRAY_SIZE(load_status));
        
        load_status[(int)data->status] += 1;
      }

      // Render stats //
      ImGui::LabelText("Count##rover_tex",   "%zu", tex_count);
      ImGui::LabelText("Unloaded#rover_tex", "%zu", load_status[(int)Resource::Load_status::NONE]);
      ImGui::LabelText("Pending##rover_tex", "%zu", load_status[(int)Resource::Load_status::PENDING]);
      ImGui::LabelText("Loaded##rover_tex",  "%zu", load_status[(int)Resource::Load_status::LOADED]);
      ImGui::LabelText("Failed##rover_tex",  "%zu", load_status[(int)Resource::Load_status::FAILED]);
    }
    else
    {
      ImGui::Text("No Texture data provided");
    }
  }

  // -- Mesh Resource Overview -- //
  if (ImGui::CollapsingHeader("Meshes##rsrc_over"))
  {
    if(mesh_rsrc != nullptr)
    {
      size_t load_status[4]{};

      // Gather stats //
      for (size_t i = 0; i < mesh_count; ++i)
      {
        const Nil_mesh *data = &mesh_rsrc[i];
        LIB_ASSERT((int)data->status < LIB_ARRAY_SIZE(load_status));
        
        load_status[(int)data->status] += 1;
      }

      // Render stats //
      ImGui::LabelText("Count##rover_mesh",   "%zu", tex_count);
      ImGui::LabelText("Unloaded#rover_mesh", "%zu", load_status[(int)Resource::Load_status::NONE]);
      ImGui::LabelText("Pending##rover_mesh", "%zu", load_status[(int)Resource::Load_status::PENDING]);
      ImGui::LabelText("Loaded##rover_mesh",  "%zu", load_status[(int)Resource::Load_status::LOADED]);
      ImGui::LabelText("Failed##rover_mesh",  "%zu", load_status[(int)Resource::Load_status::FAILED]);
    }
    else
    {
      ImGui::Text("No Mesh data provided");
    }
  }

  // -- Material Resource Overview -- //
  if (ImGui::CollapsingHeader("Materials##rsrc_over"))
  {
    if(mat_rsrc != nullptr)
    {
      ImGui::LabelText("Count##rover_mat", "%zu", mat_count);
    }
    else
    {
      ImGui::Text("No Material data provided");
    }
  }
  
  if (ImGui::CollapsingHeader("Shaders##rsrc_over"))
  {
    ImGui::Text("No UI");
  }
  
}


namespace {


void
helper_render_texture(
  const Nil_texture *tex,
  const uint32_t width,
  const uint32_t height)
{
  ImGui::Image(
    (ImTextureID)tex->platform_resource,
    ImVec2(width, height)
  );

  if(ImGui::IsItemHovered())
  {
    ImGui::SetTooltip(
      "Name: %s\nID: %d\nDimentions: %d x %d\nChannels: %d\nResource ID: %p",
      tex->name,
      tex->id,
      tex->width,
      tex->height,
      tex->components,
      (void*)tex->platform_resource
    );
  }
}


} // anon ns


void
render_resource(const Nil_texture *rsrc, const size_t count)
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
    const Nil_texture *tex = &rsrc[i];
    helper_render_texture(tex, tex_size, tex_size);
    
    if((i + 1) % (cols ))
    {
      ImGui::SameLine();
    }
  }
}


void
render_resource(const Nil_material *rsrc, const size_t count)
{
  ImGui::Text("Material Count %zu", count);

  for(size_t i = 0; i < count; ++i)
  {
    const Nil_material *data = &rsrc[i];
  
    char name[1024]{};
    sprintf(name, "Material - %s ##%zu", data->name, i + 1);
    
    if(ImGui::CollapsingHeader(name))
    {
      ImGui::Text("Disabled while cleaning up nil interface");
//      size_t tex_count = 0;
//      Nil_texture *textures;
//      nil_rsrc_texture_get_data(ctx, &tex_count, &textures);
//      
//      helper_render_texture(&textures[data->texture_01], 64, 64);
//      
//      ImGui::SameLine();
//      
//      helper_render_texture(&textures[data->texture_02], 64, 64);
//      
//      ImGui::SameLine();
//      
//      helper_render_texture(&textures[data->texture_03], 64, 64);
    
      float color[4]
      {
//        lib::color::get_channel_1f(data->color),
//        lib::color::get_channel_2f(data->color),
//        lib::color::get_channel_3f(data->color),
//        lib::color::get_channel_4f(data->color),
      };
    
      ImGui::ColorEdit4("Color##Mat", color);
    }
  }
}


void
render_resource(const Nil_mesh *rsrc, const size_t count)
{
  ImGui::Text("Mesh Count %zu", count);

  for(size_t i = 0; i < count; ++i)
  {
    const Nil_mesh *data = &rsrc[i];

    char name[1024]{};
    sprintf(name, "Mesh - %s##%zu", data->name, i + 1);
  
    if(ImGui::CollapsingHeader(name))
    {
      float columns = 3; // put position as default.

      if(data->normal_vec3)         { columns += 3.f; }
      if(data->texture_coords_vec2) { columns += 2.f; }
      if(data->color_vec4)          { columns += 4.f; }
      if(data->index)               { columns += 2.f; }

      float col_ratio = (1.f / columns) * 0.9f;

      ImGui::Text("Vertex Count: %zu", data->triangle_count);

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
        
        for (int line = 0; line < data->triangle_count; line++)
        {
          int index = line * 3;
          char pos[64]{};
          sprintf(
            pos,
            "%d) %.2f,%.2f,%.2f",
            line,
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
        
        for (int line = 0; line < data->triangle_count; line++)
        {
          int index = line * 3;
          char norm[64]{};
          sprintf(
            norm,
            "%d) %.2f,%.2f,%.2f",
            line,
            data->normal_vec3[index + 0],
            data->normal_vec3[index + 1],
            data->normal_vec3[index + 2]
          );
          ImGui::Text("%s", norm);
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
        
        for (int line = 0; line < data->triangle_count; line++)
        {
          int index = line * 2;
          char uvs[64]{};
          sprintf(
            uvs,
            "%d) %.2f,%.2f",
            line,
            data->texture_coords_vec2[index + 0],
            data->texture_coords_vec2[index + 1]
          );
          ImGui::Text("%s", uvs);
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
        
        for(int line = 0; line < data->triangle_count; line++)
        {
          int index = line * 4;
          char col[64]{};
          sprintf(
            col,
            "%d) %.2f,%.2f,%.2f,%.2f",
            line,
            data->color_vec4[index + 0],
            data->color_vec4[index + 1],
            data->color_vec4[index + 2],
            data->color_vec4[index + 3]
          );
          
          ImGui::Text("%s", col);
        }
        
        ImGui::EndChild();
        ImGui::EndGroup();
      }
      
      // Index
      if(data->index)
      {
        ImGui::BeginGroup();
        ImGui::Text("Index - (Count: %zu)", data->index_count);
        ImGui::BeginChild(
          ImGui::GetID((void*)data->index),
          ImVec2(ImGui::GetWindowWidth() * col_ratio * 2, 200.0f),
          true
        );
        
        for (int line = 0; line < data->index_count / 3; line++)
        {
          int index = line * 3;
          char uvs[64]{};
          sprintf(
            uvs,
            "%d %d %d",
            data->index[index + 0],
            data->index[index + 1],
            data->index[index + 2]
          );
          ImGui::Text("%s", uvs);
        }
        
        ImGui::EndChild();
        ImGui::EndGroup();
        
        ImGui::SameLine();
      }
      
      ImGui::Spacing();
    } // if collapsing header
  }
}


void
render_resource(const Nil_shader *rsrc, const size_t count)
{
  ImGui::Text("Shader Count %zu", count);
  
  static uint32_t tex_slider = 6;
  
  char name[32]{};
  sprintf(name, "Size: %d", 1 << tex_slider);
  ImGui::Separator();
  
  
  for(size_t i = 0; i < count; ++i)
  {
//    const Nil::Resource::Shader *shd = &rsrc[i];
  }
}



} // ns
} // ns


#endif // build guard
