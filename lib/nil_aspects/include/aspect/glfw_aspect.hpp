#ifndef GLFW_ASPECT_INCLUDED_C805890D_390C_4874_814B_73C2F77952E6
#define GLFW_ASPECT_INCLUDED_C805890D_390C_4874_814B_73C2F77952E6


#include <nil/fwd.hpp>
#include <nil/node.hpp>
#include <nil/data/keyboard.hpp>
#include <nil/task.hpp>


// --------------------------------------------------- [ GLFW Aspect Config ] --


struct GLFWwindow;


// ----------------------------------------------------- [ GLFW Aspect Data ] --


namespace Nil_ext {
namespace GLFW_Aspect {


struct Data
{
  GLFWwindow *window{nullptr};
  Nil::Node window_node{nullptr};
  
  float last_mouse[2]{0.f,0.f};
  float delta_mouse[2]{0.f, 0.f};
  
  uint32_t keys[Nil::Data::KeyCode::COUNT]{};
};


// ----------------------------------------------------- [ Aspect Interface ] --


void
start_up(Nil_ctx *ctx, void *data);


void
events(Nil_ctx *ctx, void *data);


void
shut_down(Nil_ctx *ctx, void *data);


// --------------------------------------------------------- [ Aspect Tasks ] --


void
early_think(Nil_ctx *ctx, void *data);


void
late_think(Nil_ctx *ctx, void *data);


} // ns
} // ns


#endif // inc guard
