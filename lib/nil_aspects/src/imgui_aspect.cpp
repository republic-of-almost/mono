#ifndef NIMGUI

#include <aspect/imgui_aspect.hpp>
#include <nil/aspect.hpp>
#include <nil/data/data.hpp>
#include <nil/resource/resource.hpp>
#include <nil/nil.hpp>
#include <nil/data/window.hpp>
#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <string.h>
#include <lib/utilities.hpp>
#include "imgui/imgui_resource.hpp"
#include "imgui/imgui_data.hpp"

#include <lib/bench.hpp>


namespace Nil_ext {
namespace ImGui_Aspect {


// --------------------------------------------- [ ImGUI Aspect Impl Statup ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  self->inspector_node        = Nil::Node(nullptr);
  self->show_graph            = true;
  self->show_raw_graph        = false;
  self->show_node_events      = false;
  self->show_menu             = true;
  
  self->show_data_overview    = false;
  self->show_data_audio       = false;
  self->show_data_bbox        = false;
  self->show_data_camera      = false;
  self->show_data_collider    = false;
  self->show_data_developer   = false;
  self->show_data_gamepad     = false;
  self->show_data_keyboard    = false;
  self->show_data_light       = false;
  self->show_data_logic       = false;
  self->show_data_mouse       = false;
  self->show_data_renderables = false;
  self->show_data_rigidbody   = false;
  self->show_data_transform   = false;
  self->show_data_window      = false;
  
  self->show_rsrc_overview    = false;
  self->show_rsrc_materials   = false;
  self->show_rsrc_textures    = false;
  self->show_rsrc_meshes      = false;

  // Aspects can hook into UI callbacks with developer data.
  aspect.data_types = 0;
  aspect.data_types |= Nil::Data::get_type_id(Nil::Data::Developer{});
}


// --------------------------------------------- [ ImGUI Aspect Impl Events ] --


void
events(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  // Developer Data Removed
  {
    size_t count;
    Nil::Data::Developer *data;
    Nil::Node *node;
    
    Nil::Data::events(Nil::Data::Event::REMOVED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      size_t j = 0;
    
      while(j < self->dev_nodes.size())
      {
        if(self->dev_nodes[j] == node[i])
        {
          self->dev_data.erase(self->dev_data.begin() + j);
          self->dev_nodes.erase(self->dev_nodes.begin() + j);
          break;
        }
        else
        {
          ++j;
        }
      }
    }
  } // removed dev data

  // Developer Data Added
  {
    size_t count;
    Nil::Data::Developer *data;
    Nil::Node *node;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      if(data[i].type_id == 1)
      {
        bool duplicate = false;
      
        for(Nil::Node &n : self->dev_nodes)
        {
          if(n == node[i])
          {
            duplicate = true;
            break;
          }
        }
      
        if(!duplicate)
        {
          self->dev_data.emplace_back(data[i]);
          self->dev_nodes.emplace_back(node[i]);
        }
      }
    }
  } // new dev data
  
  // Developer Data Modified
  {
    size_t count;
    Nil::Data::Developer *data;
    Nil::Node *node;
    
    Nil::Data::events(Nil::Data::Event::UPDATED, &count, &data, &node);
    
    for(size_t i = 0; i < count; ++i)
    {
      size_t j = 0;
    
      while(j < self->dev_nodes.size())
      {
        if(self->dev_nodes[j] == node[i])
        {
          self->dev_data[j] = data[i];
        }
        else
        {
          ++j;
        }
      }
    }
  } // Mod dev data
}


// ---------------------------------------------- [ ImGUI Aspect Impl Think ] --


namespace {

/*
  I know I know templates, but these helpers reduce alot of the noise in this
  file.
*/

template<typename T>
inline void
inspector_data(Nil::Node &node)
{
  T data{};
  
  if(Nil::Data::has(node, data))
  {
    const char *data_name = Nil::Data::get_type_name(data);
  
    if(ImGui::CollapsingHeader(data_name))
    {
      Nil::Data::get(node, data);
      
      if(Nil::ImGUI::render_data(&data))
      {
        Nil::Data::set(node, data);
      }
    }
  }
}


template<typename T>
inline void
add_data(Nil::Node &node)
{
  T data{};

  if(!Nil::Data::has(node, data))
  {
    Nil::Data::set(node, data);
  }
}


void
render_node(const Nil::Node &node, Nil::Node &inspect)
{
  if(node.is_valid())
  {
    char name[16];
    memset(name, 0, sizeof(name));
    sprintf(name, "%s##%d", node.get_name(), node.get_id());

    const bool show_tree = ImGui::TreeNode(name);

    ImGui::SameLine(0.f);
    ImGui::Spacing();
    ImGui::SameLine(0.f);

    char insp_button[16];
    memset(insp_button, 0, sizeof(insp_button));
    sprintf(insp_button, "»##%d", node.get_id());

    const bool select_node = ImGui::SmallButton(insp_button);

    if(select_node)
    {
      inspect = node;
    }

    if(show_tree)
    {
      for(size_t i = 0; i < node.get_child_count(); ++i)
      {
        render_node(node.get_child(i), inspect);
      }

      ImGui::TreePop();
    }
  }
};


template<typename T>
inline void
render_data(bool *window)
{
  if(*window)
  {
    char name[1024]{};
    strcat(name, Nil::Data::get_type_name(T{}));
    strcat(name, " Data##DPool");
  
    ImGui::Begin(name, window);
    
    size_t count = 0;
    T *data = nullptr;
    Nil::Data::get(&count, &data);

    if(data)
    {
      Nil::ImGUI::render_data(data, count);
    }
    
    ImGui::End();
  }
}


template<typename T>
inline void
render_rsrc(bool *window)
{
  if(*window)
  {
    char name[1024]{};
    strcat(name, Nil::Resource::get_type_name(T{}));
    strcat(name, " Resource##RPool");
  
    ImGui::Begin(name, window);
    
    size_t count = 0;
    T *rsrc;
    Nil::Resource::get(&count, &rsrc);
    
    Nil::ImGUI::render_resource(rsrc, count);
    
    ImGui::End();
  }
}


} // anon ns


void
think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  BENCH_SCOPED_CPU(Logic_Think)

  Nil::Node root = Nil::Node(0, false);


  // --------------------------------------------------------------- [ Data ] --


  // Overview
  render_data<Nil::Data::Audio>(&self->show_data_audio);
//  render_data<Nil::Data::Bounding_box>(&self->show_data_bbox);
  render_data<Nil::Data::Camera>(&self->show_data_camera);
  render_data<Nil::Data::Developer>(&self->show_data_developer);
  render_data<Nil::Data::Gamepad>(&self->show_data_gamepad);
  render_data<Nil::Data::Keyboard>(&self->show_data_keyboard);
  render_data<Nil::Data::Light>(&self->show_data_light);
  render_data<Nil::Data::Logic>(&self->show_data_logic);
  render_data<Nil::Data::Mouse>(&self->show_data_mouse);
  render_data<Nil::Data::Renderable>(&self->show_data_renderables);
//  render_data<Nil::Data::Transform>(&self->show_data_transform);
  render_data<Nil::Data::Window>(&self->show_data_window);
  
  
  // ---------------------------------------------------------- [ Resources ] --
  

  // Overview
  render_rsrc<Nil::Resource::Texture>(&self->show_rsrc_textures);
  render_rsrc<Nil::Resource::Mesh>(&self->show_rsrc_meshes);
  render_rsrc<Nil::Resource::Material>(&self->show_rsrc_materials);
  

  // ---------------------------------------------------------- [ Raw Graph ] --


  /*
    Render Raw graph data
  */
  if(self->show_raw_graph)
  {
    ImGui::Begin("Raw Graph Data", &self->show_raw_graph);

    ImGui::BeginChild("graph_data_container", ImVec2(0, 180), false);

    ImGui::BeginChild("graph_data_names", ImVec2(110, 0), false);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("ID (Parent ID):");
    ImGui::Text("Depth:");
    ImGui::Separator();
    ImGui::Text("Local Pos:");
    ImGui::Text("Local Scale:");
    ImGui::Text("Local Rot:");
    ImGui::Separator();
    ImGui::Text("World Pos:");
    ImGui::Text("World Scale:");
    ImGui::Text("World Rot:");

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);

    const size_t graph_count = engine.graph_data_count();

    for(size_t i = 0; i < graph_count; ++i)
    {
      char graph_data[16];
      memset(graph_data, 0, sizeof(graph_data));
      sprintf(graph_data, "graph##%zu", i);

      ImGui::BeginChild(graph_data, ImVec2(150,0), true);

      ImGui::Text("%d(%d)", engine.graph_data_get_ids()[i], lib::bits::upper32(engine.graph_data_details()[i]));
      ImGui::Text("%d", lib::bits::lower32(engine.graph_data_details()[i]));

      ImGui::Separator();

      math::transform l_trans = engine.graph_data_local_transforms()[i];

      ImGui::Text("%.01f, %.01f, %.1f", l_trans.position.data[0], l_trans.position.data[1], l_trans.position.data[2]);
      ImGui::Text("%.01f, %.01f, %.1f", l_trans.scale.data[0], l_trans.scale.data[1], l_trans.scale.data[2]);
      ImGui::Text("%.01f, %.01f, %.1f, %.1f", l_trans.rotation.data[0], l_trans.rotation.data[1], l_trans.rotation.data[2], l_trans.rotation.data[3]);

      ImGui::Separator();

      math::transform w_trans = engine.graph_data_world_transforms()[i];

      ImGui::Text("%.01f, %.01f, %.1f", w_trans.position.data[0], w_trans.position.data[1], w_trans.position.data[2]);
      ImGui::Text("%.01f, %.01f, %.1f", w_trans.scale.data[0], w_trans.scale.data[1], w_trans.scale.data[2]);
      ImGui::Text("%.01f, %.01f, %.1f, %.1f", w_trans.rotation.data[0], w_trans.rotation.data[1], w_trans.rotation.data[2], w_trans.rotation.data[3]);

      ImGui::EndChild();

      ImGui::SameLine();
    }

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
  }


  // -------------------------------------------------------------- [ Graph ] --

  /*
    Render Graph
  */
  if(self->show_graph)
  {
    ImGui::Begin("Graph", &self->show_graph);

    for(size_t i = 0; i < root.get_child_count(); ++i)
    {
      render_node(root.get_child(i), self->inspector_node);
    }

    static std::vector<Nil::Node> add_nodes;

    ImGui::Spacing();
    ImGui::Spacing();
    if(ImGui::SmallButton("+"))
    {
      add_nodes.emplace_back(Nil::Node());
    }

    ImGui::End();
  }

  Nil::Node next_inspector_node = self->inspector_node;


  // ---------------------------------------------------------- [ Inspector ] --


  if(self->inspector_node.is_valid())
  {
    size_t count = 0;
    Nil::Data::Camera *cam = 0;
    Nil::Data::get(&count, &cam);
    
    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    
    Nil::Data::Transform trans{};
    Nil::Data::get(self->inspector_node, trans);
    
    math::transform m_trans = math::transform_init(
      math::vec3_init(trans.position),
      math::vec3_init(trans.scale),
      math::quat_init(trans.rotation)
    );
    
//    math::mat4 view = math::mat4_lookat(
//      m_trans.position,
//      math::vec3_add(m_trans.position, math::transform_fwd(m_trans)),
//      math::transform_up(m_trans)
//    );
    
    math::mat4 proj = math::mat4_projection(
      cam[0].width * io.DisplaySize.x,
      cam[0].height * io.DisplaySize.y,
      cam[0].near_plane,
      cam[0].far_plane,
      cam[0].fov
    );
    
    math::mat4 world = math::transform_get_world_matrix(m_trans);
//    math::mat4 world = math::mat4_id();
    
    ImGuizmo::Enable(true);
    
//    ImGuizmo::DrawCube(cam[0].view_mat, proj.data, world.data);
    ImGuizmo::Manipulate(
      cam[0].view_mat,
      proj.data,
      ImGuizmo::OPERATION::TRANSLATE,
      ImGuizmo::MODE::WORLD,
      world.data
    );
    
    float rot[3];
    ImGuizmo::DecomposeMatrixToComponents(world.data, trans.position, rot, trans.scale);
    
    Nil::Data::set(self->inspector_node, trans);
    
    bool inspector_open = true;
    ImGui::Begin("Inspector", &inspector_open);
    
    ImGui::Text("Node Information");
    
    char name_buf[16]{0};
    strcat(name_buf, self->inspector_node.get_name());
    
    if(ImGui::InputText("Name##Node", name_buf, 16))
    {
      self->inspector_node.set_name(name_buf);
    }
    
    uint32_t node_id = self->inspector_node.get_id();
    ImGui::InputInt("ID", (int*)&node_id, 0, 0, ImGuiInputTextFlags_ReadOnly);

    /*
      Relationships
    */
    {
      Nil::Node parent_node = self->inspector_node.get_parent();

      if(parent_node.is_valid())
      {
        char parent_name[32]{0};
        strcat(parent_name, self->inspector_node.get_parent().get_name());
        strcat(parent_name, "##Node");

        ImGui::Text("Parent:");
        ImGui::SameLine();

        if(ImGui::SmallButton(parent_name))
        {
          next_inspector_node = parent_node;
        }
      }

      const size_t child_count = self->inspector_node.get_child_count();

      if(child_count)
      {
        ImGui::Text("Children:");

        for(size_t i = 0; i < child_count; ++i)
        {
          Nil::Node child_node = self->inspector_node.get_child(i);

          ImGui::SameLine();

          char child_name[32]{0};
          strcat(child_name, child_node.get_name());
          strcat(child_name, "##Node");

          if(ImGui::SmallButton(child_name))
          {
            next_inspector_node = child_node;
          }
        }
      }
    }


    // ------------------------------------------- [ Default Inspector Data ] --

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Node Data");
    
    // Default Data
    {
      inspector_data<Nil::Data::Transform>(self->inspector_node);
      inspector_data<Nil::Data::Bounding_box>(self->inspector_node);
    }
    
    // Additional Data
    {
      inspector_data<Nil::Data::Audio>(self->inspector_node);
      inspector_data<Nil::Data::Camera>(self->inspector_node);
      inspector_data<Nil::Data::Developer>(self->inspector_node);
      inspector_data<Nil::Data::Gamepad>(self->inspector_node);
      inspector_data<Nil::Data::Keyboard>(self->inspector_node);
      inspector_data<Nil::Data::Light>(self->inspector_node);
      inspector_data<Nil::Data::Logic>(self->inspector_node);
      inspector_data<Nil::Data::Mouse>(self->inspector_node);
      inspector_data<Nil::Data::Renderable>(self->inspector_node);
      inspector_data<Nil::Data::Window>(self->inspector_node);
    }


    // --------------------------------------------------------- [ Add Data ] --

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("Add Data");

    const char *items[] {
      "Select Data",
      "Audio",
      "BoundingBox",
      "Camera",
      "Collider",
      "Developer",
      "Gamepad",
      "Keyboard",
      "Light",
      "Logic",
      "Renderable",
      "Mouse",
      "Rigidbody",
      "Transform",
      "Window",
    };
    
    const size_t items_count = sizeof(items) / sizeof(decltype(items[0]));
    
    
    using adder_fn = void(*)(Nil::Node &node);
    
    adder_fn adders[] {
      nullptr,
      add_data<Nil::Data::Audio>,
      add_data<Nil::Data::Bounding_box>,
      add_data<Nil::Data::Camera>,
      add_data<Nil::Data::Developer>,
      add_data<Nil::Data::Gamepad>,
      add_data<Nil::Data::Keyboard>,
      add_data<Nil::Data::Light>,
      add_data<Nil::Data::Logic>,
      add_data<Nil::Data::Renderable>,
      add_data<Nil::Data::Mouse>,
      add_data<Nil::Data::Transform>,
      add_data<Nil::Data::Window>,
    };
    
    const size_t adders_count = sizeof(adders) / sizeof(decltype(adders[0]));
    
    LIB_ASSERT(adders_count == items_count);

    int item = 0;
    if(ImGui::Combo("Data##New", &item, items, items_count))
    {
      if(item && item < adders_count && adders[item])
      {
        adders[item](self->inspector_node);
      }
    }

    // ------------------------------------------------------ [ Delete Node ] --

    ImGui::Spacing();
    ImGui::Separator();

    // Delete the node?
    if(ImGui::Button("Delete Node"))
    {
      ImGui::OpenPopup("Delete");
    }

    // Destroy Node Confirmation.
    if (ImGui::BeginPopupModal("Delete", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
      ImGui::Text("Destroy Node?\n");
      ImGui::Separator();

      if (ImGui::Button("OK", ImVec2(120,0)))
      {
        self->inspector_node.destroy();
        ImGui::CloseCurrentPopup();
      }

      ImGui::SameLine();

      if (ImGui::Button("Cancel", ImVec2(120,0)))
      {
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }


    ImGui::End();

    self->inspector_node = next_inspector_node;

    if(!inspector_open)
    {
      self->inspector_node = Nil::Node(nullptr);
    }
  }


  // ---------------------------------------------------------- [ Main Menu ] --


  if(self->show_menu)
  {
    ImGui::BeginMainMenuBar();

    if(ImGui::BeginMenu("Nil"))
    {
      ImGui::MenuItem("Graph", nullptr, &self->show_graph);
      ImGui::MenuItem("Graph-Raw", nullptr, &self->show_raw_graph);
      
      if(ImGui::BeginMenu("Data##NilMenu"))
      {
        ImGui::MenuItem("Overview##NMenu",  nullptr, &self->show_data_overview);
        ImGui::Separator();
        ImGui::MenuItem("Audio##NMenu",       nullptr, &self->show_data_audio);
        ImGui::MenuItem("BoundingBox##NMenu", nullptr, &self->show_data_bbox);
        ImGui::MenuItem("Camera##NMenu",      nullptr, &self->show_data_camera);
        ImGui::MenuItem("Collider##NMenu",    nullptr, &self->show_data_collider);
        ImGui::MenuItem("Developer##NMenu",   nullptr, &self->show_data_developer);
        ImGui::MenuItem("Gamepad##NMenu",     nullptr, &self->show_data_gamepad);
        ImGui::MenuItem("Keyboard##NMenu",    nullptr, &self->show_data_keyboard);
        ImGui::MenuItem("Light##NMenu",       nullptr, &self->show_data_light);
        ImGui::MenuItem("Logic##NMenu",       nullptr, &self->show_data_logic);
        ImGui::MenuItem("Mouse##NMenu",       nullptr, &self->show_data_mouse);
        ImGui::MenuItem("Renderable##NMenu",  nullptr, &self->show_data_renderables);
        ImGui::MenuItem("Rigidbody##NMenu",   nullptr, &self->show_data_rigidbody);
        ImGui::MenuItem("Transform##NMenu",   nullptr, &self->show_data_transform);
        ImGui::MenuItem("Window##NMenu",      nullptr, &self->show_data_window);
        
        ImGui::EndMenu();
      }
      
      if(ImGui::BeginMenu("Resources##NilMenu"))
      {
        ImGui::MenuItem("Overview##NMenu",  nullptr, &self->show_rsrc_overview);
        ImGui::Separator();
      
        ImGui::MenuItem("Materials##NMenu", nullptr, &self->show_rsrc_materials);
        ImGui::MenuItem("Textures##NMenu",  nullptr, &self->show_rsrc_textures);
        ImGui::MenuItem("Meshes##NMenu",    nullptr, &self->show_rsrc_meshes);
        
        ImGui::EndMenu();
      }
      
      ImGui::MenuItem("Node Events", nullptr, &self->show_node_events);

      ImGui::Separator();

      if(ImGui::MenuItem("Quit"))
      {
        aspect.want_to_quit = true;
      }

      ImGui::EndMenu();
    }
    
    if(ImGui::BeginMenu("Lib"))
    {
      ImGui::MenuItem("Memory", nullptr, &self->show_lib_memory);
      
      ImGui::EndMenu();
    }

    for(auto &menu : self->dev_data)
    {
      using fn = void(*)(uintptr_t user_data);

      ((fn)menu.aux_01)(menu.aux_02);
    }

    ImGui::EndMainMenuBar();
  }


  // --------------------------------------------------------- [ Other Menu ] --

  for(auto &menu : self->dev_data)
  {
    using fn = void(*)(uintptr_t user_data);
    
    if(menu.aux_03)
    {
      ((fn)menu.aux_03)(menu.aux_04);
    }
  }
  
  
  // ----------------------------------------------------------- [ Lib Menu ] --

  if(self->show_lib_memory)
  {
    ImGui::Begin("Lib Memory Viewer", &self->show_lib_memory);
    {
      ImGui::Text("Bucket Capacity: %zu", lib::mem::bucket_capacity());
      ImGui::Text("Bucket Stride: %zu", lib::mem::bucket_stride());
      ImGui::Text("Buckets Used: %zu", lib::mem::buckets_in_use());
      
      ImGui::End();
    }
  }
}


} // ns
} // ns


#endif
