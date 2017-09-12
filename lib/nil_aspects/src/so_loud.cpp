#include <aspect/so_loud.hpp>
#include <nil/resource/audio.hpp>
#include <nil/data/audio.hpp>
#include <nil/task.hpp>
#include <nil/aspect.hpp>
#include <lib/assert.hpp>


namespace Nil_ext {
namespace SoLoud_Aspect {


// ------------------------------------------------------ [ SoLoud Lifetime ] --


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
  
  // -- Update Resources -- //
  {
    /*
      TODO: This task should only be fired if we know there are
      resources to update.
    */
    Nil::Task::cpu_task(
      Nil::Task::CPU::EARLY_THINK,
      aspect.user_data,
      resource_update
    );
  }
  
  // -- Update Players -- //
  {
    /*
      TODO: This task should only be fired if we know there are
      players to update.
    */
    Nil::Task::cpu_task(
      Nil::Task::CPU::THINK,
      aspect.user_data,
      player_update
    );
  }
}


// -------------------------------------------------------- [ So Loud Tasks ] --


void
resource_update(Nil::Engine &engine, uintptr_t user_data)
{
  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);

  // If new Audio Files then load
  Nil::Resource::Audio *audio_data  = nullptr;
  size_t audio_data_count = 0;

  {
    Nil::Resource::get(&audio_data_count, &audio_data);
    
    for(size_t i = 0; i < audio_data_count; ++i)
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
          }
          else
          {
            audio->status = Nil::Resource::Audio::FAILED;
          }
        }
      }
    }
  }

}


void
player_update(Nil::Engine &engine, uintptr_t user_data)
{
  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);

  Nil::Resource::Audio *audio_data  = nullptr;
  size_t audio_data_count = 0;
  
  Nil::Resource::get(&audio_data_count, &audio_data);
  
  // If new Audio players then play
  {
    Nil::Data::Audio *audio_players = nullptr;
    size_t count = 0;
    
    Nil::Data::get(&count, &audio_players);
    
    for(size_t i = 0; i < count; ++i)
    {
      Nil::Data::Audio *audio = &audio_players[i];
      
      if(audio->request_state == Nil::Data::Audio::PLAY)
      {
        SoLoud::Wav *sample = (SoLoud::Wav*)audio_data[audio->audio_id].platform_resource;
        
        auto han = self->soloud.play(*sample);
        self->soloud.setVolume(han, 1.f);
        
        audio->current_state = Nil::Data::Audio::PLAYING;
        audio->request_state = Nil::Data::Audio::NO_REQ_STATE;
      }
    }
  }

}


} // ns
} // ns
