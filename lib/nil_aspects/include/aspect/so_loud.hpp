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
start_up(Nil_ctx *ctx, void *data);


void
think(Nil_ctx *ctx, void *data);


void
shut_down(Nil_ctx *ctx, void *data);


// --------------------------------------------------------- [ SoLoud Tasks ] --


void
resource_update(Nil_ctx *ctx, void *data);


void
player_update(Nil_ctx *ctx, void *data);


// ---------------------------------------------------------- [ Renderer UI ] --


#ifndef NIMGUI
void
ui_menu(Nil_ctx *ctx, void *data);


void
ui_window(Nil_ctx *ctx, void *data);
#endif


} // ns
} // ns

#endif // inc guard
