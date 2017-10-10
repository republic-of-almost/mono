#include <aspect/aspects.hpp>
#include <aspect/glfw_aspect.hpp>
#include <aspect/roa_aspect.hpp>
#include <aspect/renderer_aspect.hpp>
#include <aspect/so_loud.hpp>

#ifndef NIMGUI
#include <aspect/imgui_aspect.hpp>
#endif

#include <nil/aspect.hpp>
#include <nil/nil.hpp>
#include <assert.h>
#include <stddef.h>


namespace Nil_ext {


void
load_aspects(Nil_ctx *ctx)
{
  /*
    GLFW Aspect
    --
    Deals with the Input/Window.
  */
  {
    static GLFW_data glfw;
    Nil_aspect aspect{};
    aspect.startup    = glfw_aspect_startup;
    aspect.tick       = glfw_aspect_tick;
    aspect.shutdown   = glfw_aspect_shutdown;
    aspect.data       = (void*)&glfw;

    nil_ctx_add_aspect(ctx, aspect);
  }

  /*
    ROV Aspect
    --
    Renderer
  */
  {
    static Nil_ext::ROV_Aspect::Data rov;
    Nil_aspect aspect{};
    aspect.startup    = Nil_ext::ROV_Aspect::start_up;
    aspect.tick       = Nil_ext::ROV_Aspect::events;
    aspect.shutdown   = Nil_ext::ROV_Aspect::shut_down;
    aspect.data       = (void*)&rov;

    nil_ctx_add_aspect(ctx, aspect);
  }

  /*
    Logic Aspect
  */
  {
    static Nil_ext::ROA_Aspect::Data logic;
    Nil_aspect aspect{};

    aspect.startup    = Nil_ext::ROA_Aspect::start_up;
    aspect.tick       = Nil_ext::ROA_Aspect::events;
    aspect.shutdown   = Nil_ext::ROA_Aspect::shut_down;
    aspect.data       = (void*)&logic;

    nil_ctx_add_aspect(ctx, aspect);
  }
  
  /*
    Audio
  */
  {
    static Nil_ext::SoLoud_Aspect::Data soloud;
    Nil_aspect aspect{};
    
    aspect.startup  = Nil_ext::SoLoud_Aspect::start_up;
    aspect.tick     = Nil_ext::SoLoud_Aspect::think;
    aspect.shutdown = Nil_ext::SoLoud_Aspect::shut_down;
    aspect.data     = (void*)&soloud;
    
    nil_ctx_add_aspect(ctx, aspect);
  }
  
  /*
    ImGui Aspect
    --
    Debugging UI
  */
  #ifndef NIMGUI
  {
    static Nil_ext::ImGui_Aspect::Data imgui;
    Nil_aspect aspect{};

    aspect.startup  = Nil_ext::ImGui_Aspect::start_up;
    aspect.tick     = Nil_ext::ImGui_Aspect::events;
    aspect.shutdown = Nil_ext::ImGui_Aspect::shut_down;
    aspect.data     = (void*)&imgui;

    nil_ctx_add_aspect(ctx, aspect);
  }
  #endif
  
}


} // ns
