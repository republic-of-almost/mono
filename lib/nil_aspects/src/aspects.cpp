#include <aspect/aspects.hpp>

#include <nil/aspect.hpp>
#include <nil/nil.hpp>

#include <aspect/glfw_aspect.hpp>


#ifndef NIMGUI
#include <aspect/imgui_aspect.hpp>
#endif

#include <aspect/renderer_aspect.hpp>
#include <aspect/logic_aspect.hpp>

#include <assert.h>
#include <stddef.h>


namespace Nil_ext {


void
load_aspects(Nil::Engine &nil_engine)
{
  static Nil_ext::GLFW_Aspect::Data glfw;
  {
    Nil::Aspect aspect{};
    aspect.start_up_fn    = Nil_ext::GLFW_Aspect::start_up;
    aspect.events_fn      = Nil_ext::GLFW_Aspect::events;
    aspect.early_think_fn = Nil_ext::GLFW_Aspect::early_think;
    aspect.late_think_fn  = Nil_ext::GLFW_Aspect::late_think;
    aspect.user_data      = (uintptr_t)&glfw;

    nil_engine.add_aspect(aspect);
  }

  /*
    ROV Aspect
    --
    Renderer
  */
  static Nil_ext::ROV_Aspect::Data rov;
  {
    Nil::Aspect rov_aspect{};

    rov_aspect.start_up_fn    = Nil_ext::ROV_Aspect::start_up;
    rov_aspect.events_fn      = Nil_ext::ROV_Aspect::events;
    rov_aspect.early_think_fn = Nil_ext::ROV_Aspect::early_think;
    rov_aspect.think_fn       = Nil_ext::ROV_Aspect::think;
    rov_aspect.user_data      = (uintptr_t)&rov;

    nil_engine.add_aspect(rov_aspect);
  }

  /*
    Logic Aspect
  */
  static Nil_ext::Logic_aspect::Data logic;
  {
    Nil::Aspect logic_aspect{};

    logic_aspect.start_up_fn    = Nil_ext::Logic_aspect::start_up;
    logic_aspect.events_fn      = Nil_ext::Logic_aspect::events;
    logic_aspect.early_think_fn = Nil_ext::Logic_aspect::early_think;
    logic_aspect.think_fn       = Nil_ext::Logic_aspect::think;
    logic_aspect.user_data      = (uintptr_t)&logic;

    nil_engine.add_aspect(logic_aspect);
  }

  /*
    ImGui Aspect
    --
    Debugging UI
  */
  #ifndef NIMGUI
  static Nil_ext::ImGui_Aspect::Data imgui;
  {
    Nil::Aspect imgui_aspect{};

    imgui_aspect.start_up_fn = Nil_ext::ImGui_Aspect::start_up;
    imgui_aspect.events_fn   = Nil_ext::ImGui_Aspect::events;
    imgui_aspect.think_fn    = Nil_ext::ImGui_Aspect::think;
    imgui_aspect.user_data   = (uintptr_t)&imgui;

    nil_engine.add_aspect(imgui_aspect);
  }
  #endif
}


} // ns
