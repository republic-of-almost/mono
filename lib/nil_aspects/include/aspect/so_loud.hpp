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
  SoLoud::Wav sample;    // One sample
  SoLoud::handle audio;
};

// ----------------------------------------------------- [ Aspect Interface ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect);


void
think(Nil::Engine &engine, Nil::Aspect &aspect);


} // ns
} // ns


#endif // inc guard
