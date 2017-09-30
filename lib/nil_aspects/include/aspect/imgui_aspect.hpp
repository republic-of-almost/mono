#ifndef NIMGUI

#ifndef IMGUI_ASPECT_INCLUDED_76A60511_5EA9_453A_A2BF_6A7EC1E35B08
#define IMGUI_ASPECT_INCLUDED_76A60511_5EA9_453A_A2BF_6A7EC1E35B08


#include <nil/fwd.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <nil/task.hpp>
#include <vector>

namespace Nil_ext {
namespace ImGui_Aspect {


// -------------------------------------------------- [ ImGUI Aspect Config ] --


/*
  To hook onto UI add developer data with this type id.
  Put callbacks for Imgui
  AUX 01 uintptr_t For Aspects Debug Menu.
  AUX 02 uintptr_t For Aspects Window Render.
*/
constexpr uint32_t developer_type_id = 1;


// ---------------------------------------------------- [ ImGUI Aspect Data ] --


struct Data
{
  Nil::Node inspector_node{nullptr};
  bool show_graph;
  bool show_raw_graph;
  bool show_node_events;
  bool show_menu;
  
  bool show_lib_memory;
  
  bool show_data_overview;
  bool show_data_audio;
  bool show_data_bbox;
  bool show_data_camera;
  bool show_data_collider;
  bool show_data_developer;
  bool show_data_gamepad;
  bool show_data_keyboard;
  bool show_data_light;
  bool show_data_logic;
  bool show_data_mouse;
  bool show_data_renderables;
  bool show_data_rigidbody;
  bool show_data_transform;
  bool show_data_window;
  
  bool show_rsrc_overview;
  bool show_rsrc_materials;
  bool show_rsrc_textures;
  bool show_rsrc_meshes;
};


// ----------------------------------------------- [ ImGUI Aspect Interface ] --


void
start_up(Nil_ctx *ctx, void *data);


void
events(Nil_ctx *ctx, void *data);


// ---------------------------------------------------------------- [ Tasks ] --


void
think(Nil_ctx *ctx, void *data);


} // ns
} // ns


#endif // inc guard

#endif // #ifndef IMGUI_ASPECT
