#ifndef SOLOUD_ASPECT_INCLUDED_
#define SOLOUD_ASPECT_INCLUDED_


#include <nil/nil.hpp>
#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>


namespace Nil_ext {
namespace SoLoud_Aspect {


struct Data
{
  SoLoud::Soloud soloud; // Engine core
  SoLoud::handle audio;
  
  // SoLoud needs Move/Copy to be container friendly.
  // Until then we are just heap alloc'ing them and adding them to the
  // platform id.
};

// ----------------------------------------------------- [ Aspect Interface ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect);


void
think(Nil::Engine &engine, Nil::Aspect &aspect);


} // ns
} // ns


#endif // inc guard
