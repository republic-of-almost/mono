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
start_up(Nil_ctx *ctx, void *data)
{
  Data *self = reinterpret_cast<Data*>(data);
  
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
think(Nil_ctx *ctx, void *data)
{
  Data *self = reinterpret_cast<Data*>(data);
  LIB_ASSERT(self);
  
  // -- Update Resources -- //
  {
    /*
      TODO: This task should only be fired if we know there are
      resources to update.
    */
    nil_task_cpu_add(
      ctx,
      NIL_CPU_TASK_EARLY_THINK,
      resource_update,
      (void*)self
    );
  }
  
  // -- Update Players -- //
  {
    /*
      TODO: This task should only be fired if we know there are
      players to update.
    */
    nil_task_cpu_add(
      ctx,
      NIL_CPU_TASK_EARLY_THINK,
      player_update,
      (void*)self
    );
  }
}


void
shut_down(Nil_ctx *ctx, void *data)
{
}


// -------------------------------------------------------- [ So Loud Tasks ] --


void
resource_update(Nil_ctx *ctx, void *data)
{
  Data *self = reinterpret_cast<Data*>(data);
  LIB_ASSERT(self);

  // If new Audio Files then load
  Nil_audio_src *audio_data  = nullptr;
  size_t audio_data_count = 0;

  nil_rsrc_audio_src_get_data(ctx, &audio_data_count, &audio_data);
  
  for(size_t i = 0; i < audio_data_count; ++i)
  {
    Nil_audio_src *audio = &audio_data[i];
  
    if(audio->status == NIL_RSRC_STATUS_PENDING)
    {
      if(audio->data_type == NIL_DATA_FILENAME)
      {
        const char *filename = (char*)audio->data;
        
        if(filename)
        {
          SoLoud::Wav *sample = new SoLoud::Wav;
          
          sample->load(filename);
          audio->status = NIL_RSRC_STATUS_LOADED;
          audio->platform_resource = (uintptr_t)sample;
        }
        else
        {
          audio->status = NIL_RSRC_STATUS_FAILED;
        }
      }
    }
  }


}


void
player_update(Nil_ctx *ctx, void *data)
{
  Data *self = reinterpret_cast<Data*>(data);
  LIB_ASSERT(self);

  Nil_audio_src *audio_data  = nullptr;
  size_t audio_data_count = 0;

  nil_rsrc_audio_src_get_data(ctx, &audio_data_count, &audio_data);
  
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
ui_menu(Nil_ctx *ctx, void *data)
{
  Nil_ext::SoLoud_Aspect::Data *self(
    reinterpret_cast<Nil_ext::SoLoud_Aspect::Data*>(data)
  );
  LIB_ASSERT(self);

  if(ImGui::BeginMenu("Audio"))
  {
    ImGui::MenuItem("Show Info", nullptr, &self->show_info);

    ImGui::EndMenu();
  }
}


void
ui_window(Nil_ctx *ctx, void *data)
{
  Nil_ext::SoLoud_Aspect::Data *self(
    reinterpret_cast<Nil_ext::SoLoud_Aspect::Data*>(data)
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
