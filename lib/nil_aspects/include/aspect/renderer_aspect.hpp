#ifndef GL_ASPECT_INCLUDED_5D7A0FCB_0B1D_4455_9F83_E01195FBD0D7
#define GL_ASPECT_INCLUDED_5D7A0FCB_0B1D_4455_9F83_E01195FBD0D7


#include <nil/fwd.hpp>
#include <nil/node.hpp>
#include <nil/task.hpp>
#include <nil/data/bounding_box.hpp>
#include <lib/array.hpp>


namespace Nil_ext {
namespace ROV_Aspect {


// ------------------------------------------------- [ Renderer Aspect Data ] --


struct Data
{
  Nil::Node renderer{ nullptr };
  bool has_initialized = false;

  uint32_t current_viewport[2]{800, 480};

  // ROV Resource IDS //
  lib::array<uint32_t, 128> mesh_ids{};
  lib::array<uint32_t>      texture_ids{};
  uint32_t light_pack{0};
  
  #ifndef NIMGUI
  bool show_debug_options{false};
  #endif
  
  #ifndef NDEBUGLINES

  #ifndef NIL_RELEASE
  bool show_debug_lines{true};
  bool show_debug_bounding_boxes{true};
  bool show_lookat_bounding_box{true};
  bool show_lookat_cross{true};
  #else
  bool show_debug_lines{false};
  bool show_debug_bounding_boxes{false};
  bool show_lookat_bounding_box{false};
  bool show_lookat_cross{false};
  #endif

  Nil::Node debug_lines{ nullptr };
  lib::array<Nil::Data::Bounding_box> selected_bbs;
  #endif
};


// ---------------------------------------------------- [ Renderer Lifetime ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect);


void
events(Nil::Engine &engine, Nil::Aspect &aspect);


void
shut_down(Nil::Engine &engine, Nil::Aspect &aspect);


// ------------------------------------------------------- [ Renderer Tasks ] --


void
load_gpu_resources(Nil::Engine &engine, uintptr_t user_data);


void
unload_gpu_resources(Nil::Engine &engine, uintptr_t user_data);


#ifndef NDEBUGLINES
void
find_lookat_bounding_box(Nil::Engine &engine, uintptr_t user_data);
#endif


void
early_think(Nil::Engine &engine, uintptr_t user_data);


void
think(Nil::Engine &engine, uintptr_t user_data);


// ------------------------------------------------------------------- [ UI ] --


void
ui_menu(uintptr_t user_data);


void
ui_window(uintptr_t user_data);



} // ns
} // ns


#endif // inc guard
