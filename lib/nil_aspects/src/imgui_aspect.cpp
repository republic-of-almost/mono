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

    if(ImGui::Checkbox("Pause Node Events", &set.pause_node_events)) { update_settings = true; }

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
    ImGui::Text("Default Node Data");

    /*
      Transform Data
    */
    if(Nil::Data::has_transform(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Transform"))
      {
        Nil::Data::Transform trans{};
        Nil::Data::get(self->inspector_node, trans);

        bool update_transform = false;
        if(ImGui::DragFloat3("Position##Tra", trans.position, 0.1f)) { update_transform = true; }
        if(ImGui::DragFloat3("Scale##Tra",    trans.scale, 0.1f))    { update_transform = true; }
        if(ImGui::DragFloat4("Rotation##Tra", trans.rotation, 0.1f)) { update_transform = true; }

        if(update_transform)
        {
          Nil::Data::set(self->inspector_node, trans);
        }
      }
    }


    /*
      Bounding Box
    */
    if(Nil::Data::has_bounding_box(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Bounding Box"))
      {
        Nil::Data::Bounding_box box{};
        Nil::Data::get(self->inspector_node, box);

        bool update_bounding_box = false;
        if(ImGui::DragFloat3("Min##BB", box.min, 0.1f)) { update_bounding_box = true; }
        if(ImGui::DragFloat3("Max##BB", box.max, 0.1f)) { update_bounding_box = true; }

        if(update_bounding_box)
        {
          Nil::Data::set(self->inspector_node, box);
        }
      }
    }


    // --------------------------------------------- [ Other Inspector Data ] --


    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Other Node Data");

    // Transform and Bounding box are currently special cases.

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

    /*
      Audio
    */
    if(Nil::Data::has_audio(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Audio"))
      {
        Nil::Data::Audio audio{};
        Nil::Data::get(self->inspector_node, audio);
        
        bool update_audio = false;
        if(ImGui::InputInt("ID##AudioPlayer", (int*)&audio.audio_id)) { update_audio = true; }
        
        const char *request[] {
          "NONE",
          "PLAY",
          "STOP",
        };
        
        if(ImGui::Combo("Request##AudioPlayer", (int*)&audio.request_state, request, sizeof(request) / sizeof(char*)))
        {
          update_audio = true;
        }
        
        const char *state[] {
          "NONE",
          "PLAYING",
          "STOPPED",
        };

        if(ImGui::Combo("Current State##AudioPlayer", (int*)&audio.current_state, state, sizeof(state) / sizeof(char*)))
        {
          update_audio = true;
        }
        
        if(ImGui::SliderFloat("Volume##AudioPlayer", &audio.volume, 0.f, 1.f)) { update_audio = true; }
        if(ImGui::Checkbox("Looping##AudioPlayer", &audio.loop))               { update_audio = true; }
        
        // Update the Data //
        if(update_audio)
        {
          Nil::Data::set(self->inspector_node, audio);
        }
      }
    }


    /*
      Audio Resource
    */
    if(Nil::Data::has_audio_resource(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Audio Resource"))
      {
        ImGui::Text("No UI Impl");
      }
    }


    /*
      Collider
    */
    if(Nil::Data::has_collider(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Collider"))
      {
        ImGui::Text("No UI Impl");
      }
    }

    /*
      Developer Data
    */
    if(Nil::Data::has_developer(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Developer"))
      {
        Nil::Data::Developer dev{};
        Nil::Data::get(self->inspector_node, dev);

        bool update_developer = false;
        if(ImGui::InputInt("AUX0", (int*)&dev.aux_01)) { update_developer = true; }
        if(ImGui::InputInt("AUX1", (int*)&dev.aux_02)) { update_developer = true; }
        if(ImGui::InputInt("AUX2", (int*)&dev.aux_03)) { update_developer = true; }
        if(ImGui::InputInt("AUX3", (int*)&dev.aux_04)) { update_developer = true; }

        if(update_developer)
        {
          Nil::Data::set(self->inspector_node, dev);
        }
      }
    }


    /*
      Gamepad
    */
    if(Nil::Data::has_gamepad(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Gamepad"))
      {
        ImGui::Text("No UI Impl");
      }
    }


    /*
      Graphics
    */
    if(Nil::Data::has_graphics(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Graphics"))
      {
        Nil::Data::Graphics gfx{};
        Nil::Data::get(self->inspector_node, gfx);

        bool update_gfx = false;

        const char *types []
        {
          "OpenGL",
          "DirectX",
        };

        if(ImGui::Combo("API", (int*)&gfx.type, types, 2))  { update_gfx = true; }
        if(ImGui::InputInt("Major", (int*)&gfx.major))      { update_gfx = true; }
        if(ImGui::InputInt("Minor", (int*)&gfx.minor))      { update_gfx = true; }

        if(update_gfx)
        {
          Nil::Data::set(self->inspector_node, gfx);
        }
      }
    }


    /*
      Keyboard
    */
    if(Nil::Data::has_keyboard(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Keyboard"))
      {
        Nil::Data::Keyboard data{};
        Nil::Data::get(self->inspector_node, data);
        
        if(Nil::ImGUI::render_data(&data))
        {
          Nil::Data::set(self->inspector_node, data);
        }
      }
    }


    /*
      Light
    */
    if(Nil::Data::has_light(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Light"))
      {
        Nil::Data::Light light{};
        Nil::Data::get(self->inspector_node, light);
        
        if(Nil::ImGUI::render_data(&light))
        {
          Nil::Data::set(self->inspector_node, light);
        }
      }
    }


    /*
      Logic
    */
    if(Nil::Data::has_logic(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Logic"))
      {
        Nil::Data::Logic logic{};
        Nil::Data::get(self->inspector_node, logic);

        bool update_logic = false;

//        if(ImGui::InputInt("Type",  (int*)&logic.type))       { update_logic = true; }
//        if(ImGui::InputInt("ID",    (int*)&logic.logic_id))   { update_logic = true; }
//        if(ImGui::InputInt("Major", (int*)&logic.aux_01))     { update_logic = true; }
//        if(ImGui::InputInt("Minor", (int*)&logic.aux_02))     { update_logic = true; }

        if(update_logic)
        {
          Nil::Data::set(self->inspector_node, logic);
        }
      }
    }


    /*
      Material
    */
    if(Nil::Data::has_renderable(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Renderable"))
      {
        Nil::Data::Renderable mat{};
        Nil::Data::get(self->inspector_node, mat);

        bool update_mat = false;

        const char *shaders[] {
          "FULLBRIGHT",
          "LIT",
          "DIR_LIGHT",
        };

        if(ImGui::Combo("Shader", (int*)&mat.shader, shaders, sizeof(shaders) / sizeof (char*))) { update_mat = true; }

        if(ImGui::ColorEdit4("Color", mat.color))                { update_mat = true; }
        if(ImGui::InputInt("Texture 01", (int*)&mat.texture_01)) { update_mat = true; }
        if(ImGui::InputInt("Texture 02", (int*)&mat.texture_02)) { update_mat = true; }
        if(ImGui::InputInt("Texture 03", (int*)&mat.texture_03)) { update_mat = true; }
        if(ImGui::DragFloat2("UV Scale", mat.scale))             { update_mat = true; }
        if(ImGui::DragFloat2("UV Offset", mat.offset))           { update_mat = true; }
        
        // Outputs //
        ImGui::InputFloat4("MatRow0", (float*)&mat.world_mat[0]);
        ImGui::InputFloat4("MatRow1", (float*)&mat.world_mat[4]);
        ImGui::InputFloat4("MatRow2", (float*)&mat.world_mat[8]);
        ImGui::InputFloat4("MatRow3", (float*)&mat.world_mat[12]);

        if(update_mat)
        {
          Nil::Data::set(self->inspector_node, mat);
        }
      }
    }


    /*
      Mesh
    */
    if(Nil::Data::has_mesh(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Mesh"))
      {
        Nil::Data::Mesh mesh{};
        Nil::Data::get(self->inspector_node, mesh);
        
        bool update_mesh = false;
        
        if(ImGui::InputInt("Mesh ID", (int*)&mesh.mesh_id))   { update_mesh = true; }
        if(ImGui::InputInt("Index ID", (int*)&mesh.index_id)) { update_mesh = true; }
        
        if(update_mesh)
        {
          Nil::Data::set(self->inspector_node, mesh);
        }
      }
    }


    /*
      Mesh Resource
    */
    if(Nil::Data::has_mesh_resource(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Mesh_resource"))
      {
        Nil::Data::Mesh_resource mesh_resource{};
        Nil::Data::get(self->inspector_node, mesh_resource);
      
        bool update_resource = false;
        
        if(ImGui::InputInt("Mesh_id", (int*)&mesh_resource.id)) { update_resource = true; }
        

        float columns = 3; // put position as default.

        if(mesh_resource.normal_vec3)         { columns += 3.f; }
        if(mesh_resource.texture_coords_vec2) { columns += 2.f; }
        if(mesh_resource.color_vec4)          { columns += 4.f; }

        float col_ratio = (1.f / columns) * 0.9f;

        ImGui::Text("Vertex Count: %zu", mesh_resource.count);

        // Positions
        if(mesh_resource.position_vec3)
        {
          ImGui::BeginGroup();
          ImGui::Text("Positions");
          ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)0), ImVec2(ImGui::GetWindowWidth() * col_ratio * 3, 200.0f), true);
          for (int line = 0; line < mesh_resource.count; line++)
          {
            int index = line * 3;
            char pos[16];
            memset(pos, 0, sizeof(pos));
            sprintf(pos, "%.2f,%.2f,%.2f", mesh_resource.position_vec3[index + 0], mesh_resource.position_vec3[index + 1], mesh_resource.position_vec3[index + 2]);
            ImGui::Text("%s", pos);
          }
          ImGui::EndChild();
          ImGui::EndGroup();
        }

        ImGui::SameLine();

        // Normals
        if(mesh_resource.normal_vec3)
        {
          ImGui::BeginGroup();
          ImGui::Text("Normals");
          ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)1), ImVec2(ImGui::GetWindowWidth() * col_ratio * 3, 200.0f), true);
          for (int line = 0; line < mesh_resource.count; line++)
          {
            int index = line * 3;
            char pos[16];
            memset(pos, 0, sizeof(pos));
            sprintf(pos, "%.2f,%.2f,%.2f", mesh_resource.normal_vec3[index + 0], mesh_resource.normal_vec3[index + 1], mesh_resource.normal_vec3[index + 2]);
            ImGui::Text("%s", pos);
          }
          ImGui::EndChild();
          ImGui::EndGroup();
        }

        ImGui::SameLine();

        // UV
        if(mesh_resource.texture_coords_vec2)
        {
          ImGui::BeginGroup();
          ImGui::Text("UVs");
          ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)2), ImVec2(ImGui::GetWindowWidth() * col_ratio * 2, 200.0f), true);
          for (int line = 0; line < mesh_resource.count; line++)
          {
            int index = line * 2;
            char pos[16];
            memset(pos, 0, sizeof(pos));
            sprintf(pos, "%.2f,%.2f", mesh_resource.texture_coords_vec2[index + 0], mesh_resource.texture_coords_vec2[index + 1]);
            ImGui::Text("%s", pos);
          }
          ImGui::EndChild();
          ImGui::EndGroup();
        }

        ImGui::SameLine();

        // Colors
        if(mesh_resource.color_vec4)
        {
          ImGui::BeginGroup();
          ImGui::Text("Colors");
          ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)3), ImVec2(ImGui::GetWindowWidth() * col_ratio * 4, 200.0f), true);
          for (int line = 0; line < mesh_resource.count; line++)
          {
            int index = line * 4;
            char pos[16];
            memset(pos, 0, sizeof(pos));
            sprintf(pos, "%.2f,%.2f,%.2f,%.2f", mesh_resource.color_vec4[index + 0], mesh_resource.color_vec4[index + 1], mesh_resource.color_vec4[index + 2], mesh_resource.color_vec4[index + 3]);
            ImGui::Text("%s", pos);
          }
          ImGui::EndChild();
          ImGui::EndGroup();
          ImGui::EndChild();
          ImGui::EndGroup();
        }
        
        if(update_resource)
        {
          Nil::Data::set(self->inspector_node, mesh_resource);
        }
      }
    }


    /*
      Mouse
    */
    if(Nil::Data::has_mouse(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Mouse"))
      {
        Nil::Data::Mouse mouse{};
        Nil::Data::get(self->inspector_node, mouse);

        constexpr ImGuiInputTextFlags_ flags = ImGuiInputTextFlags_ReadOnly;

        bool update_input = false;

        ImGui::InputInt2("Pos",   (int*)mouse.position, flags);
        ImGui::InputInt2("Delta", (int*)mouse.delta,    flags);
        if(ImGui::Checkbox("Capture", &mouse.capture)) { update_input = true; }

        if(update_input)
        {
          Nil::Data::set(self->inspector_node, mouse);
        }
      }
    }


    /*
      Resource
    */
    if(Nil::Data::has_resource(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Resource"))
      {
        Nil::Data::Resource resource{};
        Nil::Data::get(self->inspector_node, resource);

        ImGui::Text("Resources are readonly atm");

        constexpr ImGuiInputTextFlags_ flags = ImGuiInputTextFlags_ReadOnly;

        ImGui::InputInt("Type", (int*)&resource.type, 0, 0, flags);
        ImGui::InputText("Name", &resource.name[0], 64, flags);
        ImGui::InputInt("Ptr", (int*)&resource.data, 0, 0, flags);
      }
    }


    /*
      Rigidbody
    */
    if(Nil::Data::has_rigidbody(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Rigidbody"))
      {
        ImGui::Text("No UI Impl");
      }
    }


    /*
      Texture
    */
    if(Nil::Data::has_texture(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Texture"))
      {
        ImGui::Text("No UI Impl");
      }
    }
    
    
    /*
      Texture Resource
    */
    if(Nil::Data::has_texture_resource(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Texture Resource"))
      {
        Nil::Data::Texture_resource tex_resource{};
        Nil::Data::get(self->inspector_node, tex_resource);
        
        const int flags = ImGuiInputTextFlags_ReadOnly;
        
        bool update_resource = false;
        
        if(ImGui::InputInt("ID##TexR", (int*)&tex_resource.id)) { update_resource = true; }
        ImGui::InputInt("Components*##TexR", (int*)&tex_resource.compoents, flags);
        ImGui::InputInt("Width*##TexR", (int*)&tex_resource.width, flags);
        ImGui::InputInt("Height*##TexR", (int*)&tex_resource.height, flags);
        ImGui::InputInt("Depth*##TexR", (int*)&tex_resource.depth, flags);
        ImGui::InputInt("Dimentions*##TexR", (int*)&tex_resource.dimentions, flags);
        
        ImGui::Text("* Readonly");
        
        if(update_resource)
        {
          Nil::Data::set(self->inspector_node, tex_resource);
        }
      }
    }


    /*
      Window Data
    */
    if(Nil::Data::has_window(self->inspector_node))
    {
      if(ImGui::CollapsingHeader("Window"))
      {
        Nil::Data::Window window{};
        Nil::Data::get(self->inspector_node, window);

        bool update_window = false;
        if(ImGui::InputText("Window Title##Win", window.title, 32))          { update_window = true; }
        if(ImGui::DragInt("Width##Win", (int*)&window.width, 1, 0, 0xFFFF))  { update_window = true; }
        if(ImGui::DragInt("Height##Win", (int*)&window.height,1, 0, 0xFFFF)) { update_window = true; }
        if(ImGui::Checkbox("Fullscreen##Win", &window.fullscreen))           { update_window = true; }

        if(update_window)
        {
          Nil::Data::set(self->inspector_node, window);
        }
      }
    }

    // --------------------------------------------------------- [ Add Data ] --

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("Add Other Data");

    const size_t item_count = 18;

    const char *items[item_count] {
      "Select Data",
      "Audio",          // 1
      "AudioResource",  // 2
      "Camera",         // 3
      "Collider",       // 4
      "Developer",      // 5
      "Gamepad",        // 6
      "Graphics",       // 7
      "Keyboard",       // 8
      "Light",          // 9
      "Logic",          // 10
      "Material",       // 11
      "Mesh",           // 12
      "Mouse",          // 13
      "Resource",       // 14
      "Rigidbody",      // 15
      "Texture",        // 16
      "Window",         // 17
    };

    int item = 0;
    if(ImGui::Combo("Data##New", &item, items, item_count))
    {
      switch(item)
      {
        case(1):
        {
          if(!Nil::Data::has_audio(self->inspector_node))
          {
            Nil::Data::Audio data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(2):
        {
          if(!Nil::Data::has_audio_resource(self->inspector_node))
          {
            Nil::Data::Audio_resource data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(3):
        {
          if(!Nil::Data::has_camera(self->inspector_node))
          {
            Nil::Data::Camera data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(4):
        {
          if(!Nil::Data::has_collider(self->inspector_node))
          {
            Nil::Data::Collider data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(5):
        {
          if(!Nil::Data::has_developer(self->inspector_node))
          {
            Nil::Data::Developer data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(6):
        {
          if(!Nil::Data::has_gamepad(self->inspector_node))
          {
            Nil::Data::Gamepad data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(7):
        {
          if(!Nil::Data::has_graphics(self->inspector_node))
          {
            Nil::Data::Graphics data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(8):
        {
          if(!Nil::Data::has_keyboard(self->inspector_node))
          {
            Nil::Data::Keyboard data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(9):
        {
          if(!Nil::Data::has_light(self->inspector_node))
          {
            Nil::Data::Light data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(10):
        {
          if(!Nil::Data::has_logic(self->inspector_node))
          {
            Nil::Data::Logic data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(11):
        {
          if(!Nil::Data::has_renderable(self->inspector_node))
          {
            Nil::Data::Renderable data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(12):
        {
          if(!Nil::Data::has_mesh(self->inspector_node))
          {
            Nil::Data::Mesh data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(13):
        {
          if(!Nil::Data::has_mouse(self->inspector_node))
          {
            Nil::Data::Mouse data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(14):
        {
          if(!Nil::Data::has_resource(self->inspector_node))
          {
            Nil::Data::Resource data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(15):
        {
          if(!Nil::Data::has_rigidbody(self->inspector_node))
          {
            Nil::Data::Rigidbody data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(16):
        {
          if(!Nil::Data::has_texture(self->inspector_node))
          {
            Nil::Data::Texture data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
        case(17):
        {
          if(!Nil::Data::has_window(self->inspector_node))
          {
            Nil::Data::Window data{};
            Nil::Data::set(self->inspector_node, data);
          }
          break;
        }
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
