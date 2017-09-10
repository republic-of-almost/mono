#include <aspect/so_loud.hpp>
#include <nil/aspect.hpp>


namespace Nil_ext {
namespace SoLoud_Aspect {



void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *data = reinterpret_cast<Data*>(aspect.user_data);
  
  data->soloud.init();
  
  auto r = data->sample.load("/Users/PhilCK/Desktop/test.ogg");
  
  data->audio = data->soloud.play(data->sample);
  data->soloud.setVolume(data->audio, 1.f);
}


void
think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *data = reinterpret_cast<Data*>(aspect.user_data);

}


} // ns
} // ns
