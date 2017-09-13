#include <aspect/so_loud.hpp>
#include <nil/resource/audio.hpp>
#include <nil/data/audio.hpp>
#include <nil/task.hpp>
#include <nil/aspect.hpp>
#include <lib/assert.hpp>

#ifndef NIMGUI
#include <imgui/imgui.h>
#include <nil/data/developer.hpp>
#endif


namespace Nil_ext {
namespace SoLoud_Aspect {


// ------------------------------------------------------ [ SoLoud Lifetime ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  
  self->soloud.init();
  
  #ifndef NIMGUI
  Nil::Node audio_ui;
  self->audio_ui = static_cast<Nil::Node&&>(audio_ui);
  self->audio_ui.set_name("Audio");

  Nil::Data::Developer dev{};
  dev.type_id = 1;
  dev.aux_01  = (uintptr_t)ui_menu;
  dev.aux_02  = (uintptr_t)self;
  dev.aux_03  = (uintptr_t)ui_window;
  dev.aux_04  = (uintptr_t)self;

  Nil::Data::set(self->audio_ui, dev);
  #endif
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

  Nil::Resource::Audio *audio_data = nullptr;
  size_t audio_data_count          = 0;
  
  Nil::Resource::get(&audio_data_count, &audio_data);
  
  // If new Audio players then play
  {
    Nil::Data::Audio *audio_players = nullptr;
    size_t count = 0;
    
    Nil::Data::get(&count, &audio_players);
    
    for(size_t i = 0; i < count; ++i)
    {
      Nil::Data::Audio *audio = &audio_players[i];
      
      /*
        Check if they are playing
      */
      const bool has_stream = !!audio->user_data;
      
      if(has_stream)
      {
        const SoLoud::handle handle = (SoLoud::handle)audio->user_data;
        const bool valid_handle     = self->soloud.isValidVoiceHandle(handle);
        
        /*
          Handles become invalid when the audio finishes playing.
        */
        if(!valid_handle)
        {
          audio->current_state = Nil::Data::Audio::STOPPED;
          audio->user_data     = 0;
        }
      }
      
      /*
        Should play
      */
      const bool wants_to_play = audio->request_state == Nil::Data::Audio::PLAY;
      const bool is_playing    = audio->current_state == Nil::Data::Audio::PLAYING;
      
      if(wants_to_play && !is_playing)
      {
        SoLoud::Wav *sample = (SoLoud::Wav*)audio_data[audio->audio_id].platform_resource;
        
        audio->user_data = (uintptr_t)self->soloud.play(*sample);
        self->soloud.setVolume(audio->user_data, 1.f);
        
        audio->current_state = Nil::Data::Audio::PLAYING;
        audio->request_state = Nil::Data::Audio::NO_REQ_STATE;
      }
      else if(wants_to_play && is_playing)
      {
        // Don't queue up requests, other wise when a long sounds stops it
        // starts again.
        audio->request_state = Nil::Data::Audio::NO_REQ_STATE;
      }
    }
  }
}


// ---------------------------------------------------------- [ Renderer UI ] --

#ifndef NIMGUI
void
ui_menu(uintptr_t user_data)
{
  Nil_ext::SoLoud_Aspect::Data *self(
    reinterpret_cast<Nil_ext::SoLoud_Aspect::Data*>(user_data)
  );
  LIB_ASSERT(self);

  if(ImGui::BeginMenu("Audio"))
  {
    ImGui::MenuItem("Show Info", nullptr, &self->show_info);

    ImGui::EndMenu();
  }
}


void
ui_window(uintptr_t user_data)
{
  Nil_ext::SoLoud_Aspect::Data *self(
    reinterpret_cast<Nil_ext::SoLoud_Aspect::Data*>(user_data)
  );

  if(self->show_info)
  {
    ImGui::Begin("Audio Info##Audio", &self->show_info);
    
    unsigned int ver = self->soloud.getVersion();
    ImGui::Text("SoLoud Version: %d", ver);
    
    const char *backend = self->soloud.getBackendString();
    ImGui::Text("Running Backend: %s", backend);
    
    int channels = (int)self->soloud.getBackendChannels();
    ImGui::DragInt("Channels", &channels, ImGuiInputTextFlags_ReadOnly);
    
    int samplerate = (int)self->soloud.getBackendSamplerate();
    ImGui::DragInt("Samplerate", &samplerate, ImGuiInputTextFlags_ReadOnly);
    
    int buffer_size = (int)self->soloud.getBackendBufferSize();
    ImGui::DragInt("BufferSize", &buffer_size, ImGuiInputTextFlags_ReadOnly);
        
    ImGui::End();
  }
}
#endif


} // ns
} // ns
