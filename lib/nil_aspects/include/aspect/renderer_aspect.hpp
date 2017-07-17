#ifndef GL_ASPECT_INCLUDED_5D7A0FCB_0B1D_4455_9F83_E01195FBD0D7
#define GL_ASPECT_INCLUDED_5D7A0FCB_0B1D_4455_9F83_E01195FBD0D7


#include <nil/fwd.hpp>
#include <nil/node.hpp>
#include <lib/array.hpp>


// ------------------------------------------------- [ Renderer Aspect Data ] --


namespace Nil_ext {
namespace ROV_Aspect {


struct Data
{
  bool has_initialized = false;

  uint32_t current_viewport[2]{800, 480};

  lib::array<uint32_t, 128> mesh_ids{};
  lib::array<uint32_t>      texture_ids{};
  
  uint32_t light_pack{0};
  
  #ifndef NDEBUG
  Nil::Node debug_lines{nullptr};
  #endif
  
  #ifndef NIMGUI
  Nil::Node renderer{nullptr};
  bool show_debug_lines{true};
  bool show_debug_bounding_boxes{true};
  #endif
};


// -------------------------------------------- [ Renderer Aspect Interface ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect);


void
events(Nil::Engine &engine, Nil::Aspect &aspect);


void
early_think(Nil::Engine &engine, Nil::Aspect &aspect);


void
think(Nil::Engine &engine, Nil::Aspect &aspect);


void
ui_menu(uintptr_t user_data);


} // ns
} // ns


#endif // inc guard
