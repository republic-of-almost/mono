#include <aspect/so_loud.hpp>
#include <nil/aspect.hpp>
#include <lib/assert.hpp>


namespace Nil_ext {
namespace SoLoud_Aspect {



void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *data = reinterpret_cast<Data*>(aspect.user_data);
  
  data->soloud.init();
  
  data->audio = data->soloud.play(data->sample);
  data->soloud.setVolume(data->audio, 1.f);
}


void
think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *data = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(data);
  
  // If new Audio Files then load
  {
  }
  
  // If new Audio players then play
  {
  }
}


} // ns
} // ns
