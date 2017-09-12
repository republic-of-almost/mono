#include <aspect/so_loud.hpp>
#include <nil/resource/audio.hpp>
#include <nil/aspect.hpp>
#include <lib/assert.hpp>


namespace Nil_ext {
namespace SoLoud_Aspect {



void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *data = reinterpret_cast<Data*>(aspect.user_data);
  
  data->soloud.init();
  
//  data->audio = data->soloud.play(data->sample);
//  data->soloud.setVolume(data->audio, 1.f);
}


void
think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *data = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(data);
  
  // If new Audio Files then load
  {
    Nil::Resource::Audio *audio_data  = nullptr;
    size_t count = 0;
    
    Nil::Resource::get(&count, &audio_data);
    
    for(size_t i = 0; i < count; ++i)
    {
      Nil::Resource::Audio *audio = &audio_data[i];
    
      if(audio->status == Nil::Resource::Audio::PENDING)
      {
        if(audio->data_type == Nil::Resource::Audio::FILENAME)
        {
          const char *filename = (char*)audio->data;
          
          if(filename)
          {
            SoLoud::Wav *sample = new SoLoud::Wav;
            
            sample->load(filename);
            audio->status = Nil::Resource::Audio::LOADED;
            audio->platform_resource = (uintptr_t)sample;
            
            
            auto han = data->soloud.play(*sample);
            data->soloud.setVolume(han, 1.f);
          }
          else
          {
            audio->status = Nil::Resource::Audio::FAILED;
          }
        }
      }
    }
  }
  
  // If new Audio players then play
  {
  }
}


} // ns
} // ns
