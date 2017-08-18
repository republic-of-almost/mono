#include <aspect/aspects.hpp>

#include <nil/aspect.hpp>
#include <nil/nil.hpp>

#include <aspect/glfw_aspect.hpp>
#include <aspect/roa_aspect.hpp>
#include <aspect/renderer_aspect.hpp>

#ifndef NIMGUI
#include <aspect/imgui_aspect.hpp>
#endif

#include <assert.h>
#include <stddef.h>


namespace Nil_ext {


void
load_aspects(Nil::Engine &nil_engine)
{
  {
    static Nil_ext::GLFW_Aspect::Data glfw;
    Nil::Aspect aspect{};
    aspect.start_up_fn    = Nil_ext::GLFW_Aspect::start_up;
    aspect.tick_fn        = Nil_ext::GLFW_Aspect::events;
    aspect.user_data      = (uintptr_t)&glfw;

    nil_engine.add_aspect(aspect);
  }

  /*
    ROV Aspect
    --
    Renderer
  */
  {
    static Nil_ext::ROV_Aspect::Data rov;
    Nil::Aspect aspect{};
    aspect.start_up_fn    = Nil_ext::ROV_Aspect::start_up;
    aspect.tick_fn        = Nil_ext::ROV_Aspect::events;
    aspect.user_data      = (uintptr_t)&rov;

    nil_engine.add_aspect(aspect);
  }

  /*
    Logic Aspect
  */
  {
    static Nil_ext::ROA_Aspect::Data logic;
    Nil::Aspect aspect{};

    aspect.start_up_fn    = Nil_ext::ROA_Aspect::start_up;
    aspect.tick_fn        = Nil_ext::ROA_Aspect::events;
    aspect.user_data      = (uintptr_t)&logic;

    nil_engine.add_aspect(aspect);
  }
  
  /*
    ImGui Aspect
    --
    Debugging UI
  */
  #ifndef NIMGUI
  {
    static Nil_ext::ImGui_Aspect::Data imgui;
    Nil::Aspect aspect{};

    aspect.start_up_fn = Nil_ext::ImGui_Aspect::start_up;
    aspect.tick_fn     = Nil_ext::ImGui_Aspect::events;
    aspect.user_data   = (uintptr_t)&imgui;

    nil_engine.add_aspect(aspect);
  }
  #endif
}


} // ns
