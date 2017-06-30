#ifndef GLFW_ASPECT_INCLUDED_C805890D_390C_4874_814B_73C2F77952E6
#define GLFW_ASPECT_INCLUDED_C805890D_390C_4874_814B_73C2F77952E6


#include <nil/fwd.hpp>
#include <nil/node.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <nil/data/data.hpp>


// --------------------------------------------------- [ GLFW Aspect Config ] --


namespace Nil_ext {
namespace GLFW_Aspect {


// ----------------------------------------------------- [ GLFW Aspect Data ] --


struct Data
{
  GLFWwindow *window;
  Nil::Node window_node;
  
  float last_mouse_x;
  float last_mouse_y;
  
  float delta_x;
  float delta_y;
  
  uint32_t keys[Nil::Data::KeyCode::COUNT];
};


// ----------------------------------------------------- [ Aspect Interface ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect);


void
events(Nil::Engine &engine, Nil::Aspect &aspect);


void
early_think(Nil::Engine &engine, Nil::Aspect &aspect);


void
late_think(Nil::Engine &engine, Nil::Aspect &aspect);


} // ns
} // ns


#endif // inc guard
