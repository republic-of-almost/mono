//#include <GL/gl3w.h>
//
//#define ROV_GL_IMPL
#include <aspect/renderer_aspect.hpp>
#include <rov/rov.hpp>
#include <nil/nil.hpp>
#include <nil/node_event.hpp>
#include <nil/aspect.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <nil/data/window.hpp>
#include <aspect/math_nil_extensions.hpp>
#include <lib/utilities.hpp>

#ifndef NBENCHMARK
#include <remotery/Remotery.h>
#endif

#ifdef IMGIZMO_DEVELOPER_SUPPORT
#include <imguizmo/ImGuizmo.h>
#endif

#ifdef IMGUI_DEVELOPER_SUPPORT
#include <imgui/imgui.h>

#include <OpenGL/gl3.h>


// ------------------------------------------------ [ Renderer Aspect IMGUI ] --


namespace {


#ifndef NIMGUI
inline void
renderer_aspect_debug_menu(uintptr_t user_data)
{
  Nil_ext::ROV_Aspect::Data *self = reinterpret_cast<Nil_ext::ROV_Aspect::Data*>(user_data);
  LIB_ASSERT(self);

  if(ImGui::BeginMenu("ROV"))
  {
    ImGui::MenuItem("Info", nullptr, &self->renderer_show_info);
    ImGui::MenuItem("Settings", nullptr, &self->renderer_show_settings);

    ImGui::EndMenu();
  }
}


inline void
renderer_aspect_debug_window(uintptr_t user_data)
{
  Nil_ext::ROV_Aspect::Data *self = reinterpret_cast<Nil_ext::ROV_Aspect::Data*>(user_data);
  LIB_ASSERT(self);

//  if(self->renderer_show_info)
//  {
//    ImGui::Begin("Renderer Info", &self->renderer_show_info);
//
//    if(ImGui::CollapsingHeader("Cameras"))
//    {
//      ImGui::Columns(3, "cameras");
//      ImGui::Separator();
//      ImGui::Text("Node ID");     ImGui::NextColumn();
//      ImGui::Text("Clear Color"); ImGui::NextColumn();
//      ImGui::Text("Clear Depth"); ImGui::NextColumn();
//
//      ImGui::Separator();
//
//      int selected = -1;
//
//      Nil::Node_list all_cam_nodes = self->camera_nodes.all();
//
//      for(size_t i = 0; i < all_cam_nodes.size(); ++i)
//      {
//        char label[32];
//        sprintf(label, "%04d", all_cam_nodes[i].get_id());
//
//        if (ImGui::Selectable(label, selected == all_cam_nodes[i].get_id(), ImGuiSelectableFlags_SpanAllColumns))
//        {
//          selected = (int)i;
//        }
//
//        ImGui::NextColumn();
//
//        ImGui::Text("%s", self->rov_camera[i].clear_flags & rovClearFlag_Color ? "YES" : "NO"); ImGui::NextColumn();
//        ImGui::Text("%s", self->rov_camera[i].clear_flags & rovClearFlag_Depth ? "YES" : "NO"); ImGui::NextColumn();
//      }
//      ImGui::Columns(1);
//      ImGui::Separator();
//
//      ImGui::Spacing();
//
//      Nil::Node_list pending_cam_updates = self->camera_nodes.updated();
//      Nil::Node_list pending_cam_removals = self->camera_nodes.removed();
//
//      ImGui::Text(
//        "Pending Camera Updates: %s",
//        pending_cam_updates.size() ? "YES" : "NO"
//      );
//
//      ImGui::Text(
//        "Pending Camera Removals: %s",
//        pending_cam_removals.size() ? "YES" : "NO"
//      );
//    }
//
//    if(ImGui::CollapsingHeader("Draw Calls"))
//    {
//      ImGui::Columns(1, "renderables");
//      ImGui::Separator();
//      ImGui::Text("Node ID");     ImGui::NextColumn();
//
//      ImGui::Separator();
//
//      int selected = -1;
//
//      Nil::Node_list render_nodes = self->renderable_nodes.all();
//
//      for(size_t i = 0; i < self->renderables.size(); ++i)
//      {
//        char label[32];
//        sprintf(label, "%04d", render_nodes[i].get_id());
//
//        if (ImGui::Selectable(label, selected == render_nodes[i].get_id(), ImGuiSelectableFlags_SpanAllColumns))
//        {
//          selected = (int)i;
//        }
//
//        ImGui::NextColumn();
//      }
//      ImGui::Columns(1);
//      ImGui::Separator();
//
//      ImGui::Spacing();
//
//      Nil::Node_list updated_render_nodes = self->renderable_nodes.updated();
//
//      ImGui::Text(
//        "Pending Renderable Updates: %s",
//        updated_render_nodes.size() ? "YES" : "NO"
//      );
//
//      Nil::Node_list removed_render_nodes = self->renderable_nodes.removed();
//
//      ImGui::Text(
//        "Pending Renderable Removals: %s",
//        removed_render_nodes.size() ? "YES" : "NO"
//      );
//    }
//
//    ImGui::End();
//  }
//
//  if(self->renderer_show_settings)
//  {
//    ImGui::Begin("Renderer Settings", &self->renderer_show_settings);
//
//    ImGui::Checkbox("Process Pending Changes", &self->process_pending);
//
//    ImGui::End();
//  }
}
#endif


} // anon ns
#endif


namespace Nil_ext {
namespace ROV_Aspect {


// ----------------------------------------- [ Renderer Aspect Impl Startup ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  LOG_INFO("Startup Renderer Aspect")

  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  self->current_viewport[0] = 800;
  self->current_viewport[1] = 600;

  #ifndef NIMGUI
  self->renderer_show_info     = false;
  self->renderer_show_settings = false;

  self->dev_node.set_name("Renderer Dev");

  Nil::Data::Developer dev{};
  dev.type_id = 1;
  dev.aux_01 = (uintptr_t)renderer_aspect_debug_menu;
  dev.aux_02 = (uintptr_t)self;

  dev.aux_03 = (uintptr_t)renderer_aspect_debug_window;
  dev.aux_04 = (uintptr_t)self;

  Nil::Data::set(self->dev_node, dev);
  #endif
}


// ------------------------------------------ [ Renderer Aspect Impl Events ] --


void
events(Nil::Engine &engine, Nil::Aspect &aspect, Nil::Event_list &event_list)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  /*
    Updated Window
  */
  {
    size_t count = 0;
    Nil::Data::Window *win;
    
    Nil::Data::events(Nil::Data::Event::UPDATED, &count, &win, nullptr);
    
    if(count)
    {
      /* We operate on 1 window idea so grab the first */
      self->current_viewport[0] = win[0].width;
      self->current_viewport[1] = win[1].height;
    }
  }
  
  // Initialize
  {
    size_t count = 0;
    Nil::Data::Graphics *win;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &win, nullptr);
    
    if(count)
    {
      rov_initialize();
      self->has_initialized = true;
    }
  }
  
  // Added Meshs
  {
    size_t            count = 0;
    Nil::Data::Mesh   *data = nullptr;
    Nil::Node         *node = nullptr;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      
    }
  }
  
  // Added Mesh Resources
  {
    size_t                      count = 0;
    Nil::Data::Mesh_resource    *data = nullptr;
    Nil::Node                   *node = nullptr;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      self->pending_mesh_load.emplace_back(node[i]);
    }
  }
  
  // Added Texture Resource
  {
    size_t                        count = 0;
    Nil::Data::Texture_resource   *data = nullptr;
    Nil::Node                     *node = nullptr;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      self->pending_texture_load.emplace_back(node[i]);
    }
  }
  
  // Developer Thingy
  {
    size_t                count = 0;
    Nil::Data::Developer   *data = nullptr;
    Nil::Node             *node = nullptr;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      if(data[i].type_id == 2)
      {
        self->debug_lines = node[i];
      }
    }
  }
}


// ------------------------------------- [ Renderer Aspect Impl Early Think ] --


void
early_think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  rmt_ScopedCPUSample(ROV_EarlyThink, 0);

  /*
    Resources
  */
  {
    for(auto &node : self->pending_mesh_load)
    {
      Nil::Data::Mesh_resource mesh_resource{};
      Nil::Data::get(node, mesh_resource);

      if(mesh_resource.status == 0)
      {
        const uint32_t mesh = rov_createMesh(
          mesh_resource.position_vec3,
          mesh_resource.normal_vec3,
          mesh_resource.texture_coords_vec2,
          mesh_resource.count
        );
        
        if((mesh_resource.id + 1) > self->mesh_ids.size())
        {
          self->mesh_ids.resize(1 << (mesh_resource.id + 1));
        }
        
        self->mesh_ids[mesh_resource.id] = mesh;
        
        mesh_resource.status = 1;
        Nil::Data::set(node, mesh_resource);
      }
    }
    
    for(auto &node : self->pending_texture_load)
    {
      Nil::Data::Texture_resource texture_resource{};
      Nil::Data::get(node, texture_resource);
      
      if(texture_resource.status == 0)
      {
        const uint32_t texture = rov_createTexture(
          texture_resource.data,
          texture_resource.width,
          texture_resource.height,
          texture_resource.sizeof_data,
          texture_resource.compoents == 3 ? rovPixel_RGB8 : rovPixel_RGBA8
        );
        
        if((texture_resource.id + 1) > self->texture_ids.size())
        {
          self->texture_ids.resize(1 << (texture_resource.id + 1));
        }
        
        self->texture_ids[texture_resource.id] = texture;
      }
      
      texture_resource.status = 1;
      Nil::Data::set(node, texture_resource);
    }
  }
}


// -------------------------------------- [ Renderer Aspect Impl Late Think ] --


void
think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  rmt_ScopedCPUSample(ROV_Think, 0);

  if(self->has_initialized)
  {
    size_t cam_count = 0;
    Nil::Data::Camera *cameras;
    
    Nil::Data::get(&cam_count, &cameras);
    
    for(uint32_t j = 0; j < cam_count; ++j)
    {
      Nil::Data::Camera cam = cameras[j];
      
      #ifdef IMGIZMO_DEVELOPER_SUPPORT
      float world_mat[16];
      math::mat4_to_array(self->renderables.front().world, world_mat);

      ImGuiIO& io = ImGui::GetIO();
      ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
      ImGuizmo::Manipulate(
        math::mat4_get_data(cam.view),
        math::mat4_get_data(cam.proj),
        ImGuizmo::ROTATE,
        ImGuizmo::WORLD,
        world_mat
      );
      #endif
      
      uint32_t clear_flags = 0;
      if(cam.clear_color_buffer) { clear_flags |= rovClearFlag_Color; }
      if(cam.clear_depth_buffer) { clear_flags |= rovClearFlag_Depth; }

      const uint32_t viewport[4] {0,0,self->current_viewport[0], self->current_viewport[1]};
      
      const math::mat4 proj = math::mat4_projection(cam.width * self->current_viewport[0], cam.height * self->current_viewport[1], cam.near_plane, cam.far_plane, cam.fov);
      

      rov_startRenderPass(
        cam.view_mat,
        math::mat4_get_data(proj),
        viewport,
        clear_flags
      );

      size_t renderable_count = 0;
      Nil::Data::Renderable *renderables;
      
      Nil::Data::get(&renderable_count, &renderables);
    

      for(size_t i = 0; i < renderable_count; ++i)
      {
        Nil::Data::Renderable render = renderables[i];
      
        if(self->mesh_ids.size() > render.mesh_id)
        {
          rov_setColor(render.color[0], render.color[1], render.color[2], render.color[3]);
          rov_setShader(rovShader_Lit);
          
          rov_setMesh(self->mesh_ids[render.mesh_id]);
          
          if(render.texture_01)
          {
            rov_setTexture(self->texture_ids[render.texture_01], 0);
          }
          
          rov_submitMeshTransform(render.world_mat);
        }
      }
      
      // debug_lines
      if(self->debug_lines)
      {
        Nil::Data::Developer line_data{};
        Nil::Data::get(self->debug_lines, line_data);

        const float *data = (float*)line_data.aux_01;
        size_t count = (size_t)line_data.aux_02;

        LIB_ASSERT(count % 9 == 0);

        const size_t lines = count / 9;

        for(size_t i = 0; i < lines; ++i)
        {
          const size_t index = i * 9;

          rov_setColor(data[index + 6], data[index + 7], data[index + 8], 1.f);
          rov_submitLine(&data[index + 0], &data[index + 3]);
        }

        // Signal to line renderer not reset the data buffer.
        line_data.aux_02 = 0;
        Nil::Data::set(self->debug_lines, line_data);
      }
    }

    rov_execute();
  }
}


} // ns
} // ns
