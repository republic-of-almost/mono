#ifndef GL_ASPECT_INCLUDED_5D7A0FCB_0B1D_4455_9F83_E01195FBD0D7
#define GL_ASPECT_INCLUDED_5D7A0FCB_0B1D_4455_9F83_E01195FBD0D7


#include <nil/fwd.hpp>
#include <nil/node.hpp>
#include <math/math.hpp>
#include <lib/utilities.hpp>
#include <rov/rov.hpp>


namespace Nil_ext {
namespace ROV_Aspect {


// ------------------------------------------------- [ Renderer Aspect Data ] --


struct Data
{
  uint32_t current_viewport[2];

  lib::array<uint32_t, 128> mesh_ids;
  lib::array<uint32_t> texture_ids;
  
  Nil::Node debug_lines = Nil::Node(nullptr);
  
  bool has_initialized = false;
  
  uint32_t light_pack = 0;
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


} // ns
} // ns


#endif // inc guard
