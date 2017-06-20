#ifndef GL_ASPECT_INCLUDED_5D7A0FCB_0B1D_4455_9F83_E01195FBD0D7
#define GL_ASPECT_INCLUDED_5D7A0FCB_0B1D_4455_9F83_E01195FBD0D7


#include <nil/fwd.hpp>
#include <nil/node.hpp>
#include <math/math.hpp>
#include <lib/utilities.hpp>
#include <vector>


// ----------------------------------------------- [ Renderer Aspect Config ] --


#define IMGUI_DEVELOPER_SUPPORT


namespace Nil_ext {
namespace ROV_Aspect {


// ------------------------------------------------- [ Renderer Aspect Data ] --


struct Data
{
  uint32_t current_viewport[2];

  // Meshes //

  std::vector<uint32_t> mesh_ids;
  std::vector<Nil::Node> pending_mesh_load;
  
  std::vector<uint32_t> texture_ids;
  std::vector<Nil::Node> pending_texture_load;
  
  
  Nil::Node debug_lines = Nil::Node(nullptr);
  
  // Camera //
  
  struct ROV_Camera
  {
    math::mat4 view;
    math::mat4 proj;
    
    uint32_t clear_flags;
  };

  std::vector<uint32_t> camera_node_ids;
  std::vector<ROV_Camera> rov_camera;
  
  bool has_initialized = false;
  bool process_pending = true;

  #ifdef IMGUI_DEVELOPER_SUPPORT
  Nil::Node dev_node;
  bool renderer_show_info;
  bool renderer_show_settings;
  #endif
};


// -------------------------------------------- [ Renderer Aspect Interface ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect);


void
events(Nil::Engine &engine, Nil::Aspect &aspect, Nil::Event_list &event_list);


void
early_think(Nil::Engine &engine, Nil::Aspect &aspect);


void
think(Nil::Engine &engine, Nil::Aspect &aspect);


} // ns
} // ns


#endif // inc guard
