#include <aspect/aspects.hpp>

#include <nil/aspect.hpp>
#include <nil/nil.hpp>

#include <aspect/glfw_aspect.hpp>
#include <aspect/roa_aspect.hpp>
#include <aspect/renderer_aspect.hpp>

#ifndef NIMGUI
#include <aspect/imgui_aspect.hpp>
#endif

#include <aspect/so_loud.hpp>

#include <assert.h>
#include <stddef.h>


namespace Nil_ext {


void
load_aspects(Nil_ctx *ctx)
{
  {
    static Nil_ext::GLFW_Aspect::Data glfw;
    Nil_aspect aspect{};
    aspect.startup    = Nil_ext::GLFW_Aspect::start_up;
    aspect.tick       = Nil_ext::GLFW_Aspect::events;
    aspect.shutdown   = Nil_ext::GLFW_Aspect::shutdown;
    aspect.data       = (uintptr_t)&glfw;

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
