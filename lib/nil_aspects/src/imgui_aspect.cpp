#ifndef NIMGUI

#include <aspect/imgui_aspect.hpp>
#include <nil/aspect.hpp>
#include <nil/data/data.hpp>
#include <nil/node_event.hpp>
#include <nil/nil.hpp>
#include <nil/data/window.hpp>
#include <imgui/imgui.h>
//#include <imguizmo/ImGuizmo.h>
#include <string.h>
#include <lib/utilities.hpp>

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

  self->inspector_node    = Nil::Node(nullptr);
  self->show_graph        = true;
  self->show_raw_graph    = false;
  self->show_node_events  = false;
  self->show_menu         = true;
  
  self->show_data_camera       = false;
  self->show_data_renderables  = false;
  self->show_data_textures     = false;

  // Aspects can hook into UI callbacks with developer data.
  aspect.data_types = 0;
  aspect.data_types |= Nil::Data::get_type_id(Nil::Data::Developer{});
}


// --------------------------------------------- [ ImGUI Aspect Impl Events ] --


void
events(Nil::Engine &engine, Nil::Aspect &aspect, Nil::Event_list &event_list)
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


} // anon ns


void
think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  BENCH_SCOPED_CPU(Logic_Think)

  Nil::Node root = Nil::Node(0, false);

  // ----------------------------------------------------------- [ Settings ] --

  /*
    Render Settings
  */
  if(self->show_node_events)
  {
    ImGui::Begin("Node Events", &self->show_node_events);

    Nil::Engine_settings set{};
    engine.get_settings(set);

    Nil::Engine_state stat;
    engine.get_state(stat);

    bool update_settings = false;

    if(update_settings)
    {
      engine.set_settings(set);
    }

    ImGui::Spacing();

    ImGui::Text("Node Events");
    ImGui::Columns(5, "pending_events"); // 4-ways, with border
    ImGui::Separator();
    ImGui::Text("Node ID");   ImGui::NextColumn();
    ImGui::Text("Added");     ImGui::NextColumn();
    ImGui::Text("Updated");   ImGui::NextColumn();
    ImGui::Text("Moved");     ImGui::NextColumn();
    ImGui::Text("Removed");   ImGui::NextColumn();

    ImGui::Separator();

    int selected = -1;

    for (int i = 0; i < stat.node_event_count; i++)
    {
      Nil::Node event_node = Nil::Node(stat.node_events[i].node_id, false);

      char label[32];
      sprintf(label, "%04d", event_node.get_id());

      if (ImGui::Selectable(label, selected == event_node.get_id(), ImGuiSelectableFlags_SpanAllColumns))
          selected = i;

      ImGui::NextColumn();

      ImGui::Text(Nil::Event::node_added(stat.node_events[i]) ? "YES" : "NO");    ImGui::NextColumn();
      ImGui::Text(Nil::Event::node_updated(stat.node_events[i]) ? "YES" : "NO");  ImGui::NextColumn();
      ImGui::Text(Nil::Event::node_moved(stat.node_events[i]) ? "YES" : "NO");    ImGui::NextColumn();
      ImGui::Text(Nil::Event::node_removed(stat.node_events[i]) ? "YES" : "NO");  ImGui::NextColumn();
    }
    ImGui::Columns(1);

    if(selected > -1)
    {
      self->inspector_node = Nil::Node(stat.node_events[selected].node_id);
    }

    ImGui::End();
  }

  // --------------------------------------------------------------- [ Data ] --

  /*
    Render Data store
  */
  if(self->show_data_camera)
  {
    ImGui::Begin("Camera Data", &self->show_data_camera);
    
    size_t count            = 0;
    Nil::Data::Camera *cams = nullptr;
    Nil::Data::get(&count, &cams);
    
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
    
    ImGui::End();
  }


  /*
    Textures
  */
  if(self->show_data_textures)
  {
    ImGui::Begin("Texture Data", &self->show_data_textures);
    
    size_t count            = 0;
    Nil::Data::Texture_resource *textures = nullptr;
    Nil::Data::get(&count, &textures);

    ImGui::Text("Texture Count %zu", count);
    
    static uint32_t tex_slider = 6;
    char name[32]{};
    sprintf(name, "Size: %d", 1 << tex_slider);
    ImGui::SliderInt("Texture Size", (int*)&tex_slider, 6, 10, name);
    
    ImGui::Separator();
    
    const ImVec2 win_size = ImGui::GetWindowSize();
    
    const uint32_t tex_size = 1 << tex_slider;
    const uint32_t col_size_est = tex_size + 20;
    
    const uint32_t cols = math::max(((uint32_t)win_size.x / col_size_est), (uint32_t)1);
    
    for(size_t i = 0; i < count; ++i)
    {
      ImGui::Image((ImTextureID)textures[i].platform_resource, ImVec2(tex_size, tex_size));
    
      if(ImGui::IsItemHovered())
      {
        ImGui::SetTooltip("ID: %d\nDimentions: %d x %d\nChannels: %d", textures[i].id,  textures[i].width, textures[i].height, textures[i].compoents);
      }
      
      if((i + 1) % (cols ))
      {
        ImGui::SameLine();
      }
    }

    ImGui::End();
  }
  
  /*
    Renderables
  */
  if(self->show_data_renderables)
  {
    ImGui::Begin("Renderable Data", &self->show_data_renderables);

    size_t count = 0;
    Nil::Data::Renderable *renderables = nullptr;
    Nil::Data::get(&count, &renderables);
    
    ImGui::Text("Renderable Count %zu", count);
    
    for(size_t i = 0; i < count; ++i)
    {
      
    }
    

    ImGui::End();
  }

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
      inspector_data<Nil::Data::Audio_resource>(self->inspector_node);
      inspector_data<Nil::Data::Camera>(self->inspector_node);
      inspector_data<Nil::Data::Collider>(self->inspector_node);
      inspector_data<Nil::Data::Developer>(self->inspector_node);
      inspector_data<Nil::Data::Gamepad>(self->inspector_node);
      inspector_data<Nil::Data::Graphics>(self->inspector_node);
      inspector_data<Nil::Data::Keyboard>(self->inspector_node);
      inspector_data<Nil::Data::Light>(self->inspector_node);
      inspector_data<Nil::Data::Logic>(self->inspector_node);
      inspector_data<Nil::Data::Mesh>(self->inspector_node);
      inspector_data<Nil::Data::Mesh_resource>(self->inspector_node);
      inspector_data<Nil::Data::Mouse>(self->inspector_node);
      inspector_data<Nil::Data::Resource>(self->inspector_node);
      inspector_data<Nil::Data::Renderable>(self->inspector_node);
      inspector_data<Nil::Data::Rigidbody>(self->inspector_node);
      inspector_data<Nil::Data::Texture>(self->inspector_node);
      inspector_data<Nil::Data::Texture_resource>(self->inspector_node);
      inspector_data<Nil::Data::Window>(self->inspector_node);
    }


    // --------------------------------------------------------- [ Add Data ] --

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("Add Data");

    const char *items[] {
      "Select Data",
      "Audio",
      "AudioResource",
      "BoundingBox",
      "Camera",
      "Collider",
      "Developer",
      "Gamepad",
      "Graphics",
      "Keyboard",
      "Light",
      "Logic",
      "Renderable",
      "Mesh",
      "Mouse",
      "Resource",
      "Rigidbody",
      "Texture",
      "Transform",
      "Window",
    };
    
    const size_t items_count = sizeof(items) / sizeof(decltype(items[0]));
    
    
    using adder_fn = void(*)(Nil::Node &node);
    
    adder_fn adders[] {
      nullptr,
      add_data<Nil::Data::Audio>,
      add_data<Nil::Data::Audio_resource>,
      add_data<Nil::Data::Bounding_box>,
      add_data<Nil::Data::Camera>,
      add_data<Nil::Data::Collider>,
      add_data<Nil::Data::Developer>,
      add_data<Nil::Data::Gamepad>,
      add_data<Nil::Data::Graphics>,
      add_data<Nil::Data::Keyboard>,
      add_data<Nil::Data::Light>,
      add_data<Nil::Data::Logic>,
      add_data<Nil::Data::Renderable>,
      add_data<Nil::Data::Mesh>,
      add_data<Nil::Data::Mouse>,
      add_data<Nil::Data::Resource>,
      add_data<Nil::Data::Rigidbody>,
      add_data<Nil::Data::Texture>,
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
        ImGui::MenuItem("Camera##NilMenu",      nullptr, &self->show_data_camera);
        ImGui::MenuItem("Textures##NilMenu",    nullptr, &self->show_data_textures);
        ImGui::MenuItem("Renderables##NilMenu", nullptr, &self->show_data_renderables);
        
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
