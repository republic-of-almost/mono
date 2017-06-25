#include <aspect/glfw_aspect.hpp>
#include <nil/node.hpp>
#include <nil/node_event.hpp>
#include <nil/data/data.hpp>
#include <nil/data/window.hpp>
#include <nil/aspect.hpp>
#include <lib/utilities.hpp>
#include <lib/bench.hpp>
#include <math/math.hpp>
#include <stddef.h>
#include <GLFW/glfw3.h>


namespace Nil_ext {
namespace GLFW_Aspect {


// ----------------------------------------------------- [ Aspect Interface ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);

  self->window_node = Nil::Node(nullptr);

  if(!glfwInit())
  {
    LOG_FATAL("Failed to startup GFLW");
  }
  
  glfwSetErrorCallback([](int error, const char* description)
  {
    LOG_ERROR("GLFW Error - Set break point for more info.");
  });
}


void
events(Nil::Engine &engine, Nil::Aspect &aspect, Nil::Event_list &event_list)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  // Added Window
  if(!self->window_node)
  {
    size_t count = 0;
    Nil::Data::Window *data = nullptr;
    Nil::Node *nodes;
  
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &nodes);
  
    if(count)
    {
      if(nodes)
      {
        self->window_node = nodes[0];
        
//        glfwSetErrorCallback(error_callback);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        self->window = glfwCreateWindow(800, 600, "Foo", NULL, NULL);
        glfwSetWindowUserPointer(self->window, self);

        
        #ifndef __EMSCRIPTEN__
        
        glfwMakeContextCurrent(self->window);
        gl3wInit();
        
        Nil::Data::Graphics gfx{};
        gfx.type = Nil::Data::Graphics::OGL;
        
        glGetIntegerv(GL_MAJOR_VERSION, (GLint*)&gfx.major);
        glGetIntegerv(GL_MINOR_VERSION, (GLint*)&gfx.minor);
        
        #else
        
        gfx.major = 2.0;
        gfx.minor = 1.0;
        
        #endif

        Nil::Data::set(self->window_node, gfx);
        
        // Resize
        glfwSetWindowSizeCallback(
          self->window,
          [](GLFWwindow *window, int width, int height)
          {
            Data *self = reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));
            LIB_ASSERT(self);
            
            if(self->window_node)
            {
              Nil::Data::Window data;
              Nil::Data::get(self->window_node, data);
              
              data.width = width;
              data.height = height;
              
              Nil::Data::set(self->window_node, data);
            }
            else
            {
              LOG_ERROR("Failed to find node to resize the window");
            }
          }
        );
        
      }
    }
  }
}


void
early_think(Nil::Engine &engine, Nil::Aspect &aspect)
{
}


void
late_think(Nil::Engine &engine, Nil::Aspect &aspect)
{
  Data *self = reinterpret_cast<Data*>(aspect.user_data);
  LIB_ASSERT(self);
  
  if(self->window)
  {
    glfwPollEvents();
    
    glfwSwapBuffers(self->window);
  }
}


} // ns
} // ns
