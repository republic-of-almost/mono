#include <aspect/sdl_mixer_aspect.hpp>
#include <nil/aspect.hpp>
#include <nil/data/data.hpp>
#include <nil/node_event.hpp>
#include <nil/node.hpp>
#include <lib/key.hpp>
#include <lib/utilities.hpp>
#include <lib/array.hpp>
#include <lib/entity.hpp>

#ifndef NIMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>
//#include <imguizmo/ImGuizmo.h>
#endif

#include <stdint.h>

#ifndef NBENCHMARK
#include <remotery/Remotery.h>
#endif


namespace {


/*
  Callback and Data for when a channel has finished.
  Seems SDLMixer doesn't let me pass user data so need to keep this list
  global.
*/
constexpr uint32_t max_channels = 512;
lib::array<int, 32> channels_finished;


void
finished_chans(int channel)
{
  channels_finished.emplace_back(channel);
}


} // ns


// ----------------------------------------------- [ SDL Mixer Aspect IMGUI ] --


#ifndef NIMGUI


namespace {


inline void
sdl_mixer_aspect_debug_menu(uintptr_t user_data)
{
  Nil_ext::SDLMixer_Aspect::Data *self = reinterpret_cast<Nil_ext::SDLMixer_Aspect::Data*>(user_data);
  LIB_ASSERT(self);

  if(ImGui::BeginMenu("SDL_MIX"))
  {
    ImGui::MenuItem("Info", nullptr, &self->sdl_show_info);
  
    ImGui::EndMenu();
  }
}


inline void
sdl_aspect_debug_window(uintptr_t user_data)
{
  Nil_ext::SDLMixer_Aspect::Data *self = reinterpret_cast<Nil_ext::SDLMixer_Aspect::Data*>(user_data);
  LIB_ASSERT(self);

  if(self->sdl_show_info)
  {
    ImGui::Begin("Mixer Info", &self->sdl_show_info);
    
    if(ImGui::CollapsingHeader("Active Channels"))
    {
      const size_t count = self->channels.size();
      
      for(uint32_t i = 0; i < count; ++i)
      {
        const uint32_t entity = self->channels[i];
        
        if(entity)
        {
          ImGui::Text("Channel %i\n", i);
        }
      }
    }
    
    ImGui::End();
  }
}


} // ns


#endif


// ------------------------------------------------ [ SDL Mixer Aspect Impl ] --


namespace Nil_ext {
namespace SDLMixer_Aspect {


void
start_up(
  Nil::Engine &engine,
  Nil::Aspect &aspect)
{
  LOG_INFO("Startup SDL Mixer Aspect");

  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  #ifndef NIMGUI
  {
    Nil::Data::Developer dev{};
    dev.type_id = 1;
    dev.aux_01 = (uintptr_t)sdl_mixer_aspect_debug_menu;
    dev.aux_02 = (uintptr_t)self;

    dev.aux_03 = (uintptr_t)sdl_aspect_debug_window;
    dev.aux_04 = (uintptr_t)self;

    Nil::Data::set(self->dev_menu_node, dev);
  }
  #endif
}


void
events(
  Nil::Engine &engine,
  Nil::Aspect &aspect,
  Nil::Event_list &event_list)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  // SDL has been initialized
  {
    size_t               count    = 0;
    Nil::Data::Graphics *graphics = nullptr;
    Nil::Node           *node     = nullptr;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &graphics, &node);
    
    if(count)
    {
      self->initialized = true;
      
      const int init_mixer = Mix_Init(MIX_INIT_OGG|MIX_INIT_MOD);
      const int open_audio = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
      
      if(init_mixer < 0 || open_audio < 0)
      {
        LOG_FATAL("Failed to initialize Audio Mixer");
        LIB_ASSERT(false);
        
        self->initialized = false;
      }
      
      const int channels = Mix_AllocateChannels(max_channels);
      
      if(channels < 0)
      {
        LOG_FATAL("Failed to allocate Channels");
        LIB_ASSERT(false);
        
        self->initialized = false;
      }
      
      self->channels.resize(1024);
      memset(&self->channels[0], 0, sizeof(uint32_t) * self->channels.size());
      
      Mix_ChannelFinished(finished_chans);
      
      // Debugging //
      Mix_Chunk *sample = Mix_LoadWAV("/Users/PhilCK/Desktop/Lua/audio/walk.wav");
      self->samples.emplace_back(sample);
      self->sample_keys.emplace_back(uint32_t{0});
    }
  }
}

void
late_think(
  Nil::Engine &engine,
  Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  
}


} // ns
} // ns
