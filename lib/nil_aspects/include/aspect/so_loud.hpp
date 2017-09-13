#ifndef SOLOUD_ASPECT_INCLUDED_433AF29E_B0ED_4559_97FF_21001B45BB80
#define SOLOUD_ASPECT_INCLUDED_433AF29E_B0ED_4559_97FF_21001B45BB80


#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>


namespace Nil_ext {
namespace SoLoud_Aspect {


struct Data
{
  SoLoud::Soloud soloud; // Engine core
  SoLoud::handle audio;
  
  #ifndef NIMGUI
  Nil::Node audio_ui{nullptr};
  
  bool show_info{false};
  #endif
  
  // SoLoud needs Move/Copy to be container friendly.
  // Until then we are just heap alloc'ing them and adding them to the
  // platform id.
};


// ------------------------------------------------------ [ SoLoud Lifetime ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect);


void
think(Nil::Engine &engine, Nil::Aspect &aspect);


// --------------------------------------------------------- [ SoLoud Tasks ] --


void
resource_update(Nil::Engine &engine, uintptr_t user_data);


void
player_update(Nil::Engine &engine, uintptr_t user_data);


// ---------------------------------------------------------- [ Renderer UI ] --


#ifndef NIMGUI
void
ui_menu(uintptr_t user_data);


void
ui_window(uintptr_t user_data);
#endif


} // ns
} // ns

#endif // inc guard
