#ifndef GLFW_ASPECT_INCLUDED_C805890D_390C_4874_814B_73C2F77952E6
#define GLFW_ASPECT_INCLUDED_C805890D_390C_4874_814B_73C2F77952E6


#include <nil/fwd.hpp>
#include <nil/node.hpp>
#include <nil/data/keyboard.hpp>


/* ------------------------------------------------ [ GLFW Aspect Config ] -- */


struct GLFWwindow;


/* -------------------------------------------------- [ GLFW Aspect Data ] -- */


struct GLFW_data
{
  GLFWwindow *window{nullptr};
  Nil::Node window_node{nullptr};
  
  float last_mouse[2]{0.f,0.f};
  float delta_mouse[2]{0.f, 0.f};
  
  uint32_t keys[Nil::Data::KeyCode::COUNT]{};
};


/* ----------------------------------------------------- [ GLFW Lifetime ] -- */


void
glfw_aspect_startup(Nil_ctx *ctx, void *data);


void
glfw_aspect_tick(Nil_ctx *ctx, void *data);


void
glfw_aspect_shutdown(Nil_ctx *ctx, void *data);


/* ------------------------------------------------------ [ Aspect Tasks ] -- */


void
glfw_aspect_swap_buffer(Nil_ctx *ctx, void *data);


void
glfw_aspect_process_state(Nil_ctx *ctx, void *data);


#endif // inc guard
