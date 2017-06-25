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


#ifndef NIMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
//#include <imguizmo/ImGuizmo.h>
#endif


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


namespace {

inline uint32_t
glfw_key_to_nil(const int glfw_key)
{
  switch(glfw_key)
  {
    case(GLFW_KEY_A) : return Nil::Data::KeyCode::A;
    case(GLFW_KEY_B) : return Nil::Data::KeyCode::B;
    case(GLFW_KEY_C) : return Nil::Data::KeyCode::C;
    case(GLFW_KEY_D) : return Nil::Data::KeyCode::D;
    case(GLFW_KEY_E) : return Nil::Data::KeyCode::E;
    case(GLFW_KEY_F) : return Nil::Data::KeyCode::F;
    case(GLFW_KEY_G) : return Nil::Data::KeyCode::G;
    case(GLFW_KEY_H) : return Nil::Data::KeyCode::H;
    case(GLFW_KEY_I) : return Nil::Data::KeyCode::I;
    case(GLFW_KEY_J) : return Nil::Data::KeyCode::J;
    case(GLFW_KEY_K) : return Nil::Data::KeyCode::K;
    case(GLFW_KEY_L) : return Nil::Data::KeyCode::L;
    case(GLFW_KEY_M) : return Nil::Data::KeyCode::M;
    case(GLFW_KEY_N) : return Nil::Data::KeyCode::N;
    case(GLFW_KEY_O) : return Nil::Data::KeyCode::O;
    case(GLFW_KEY_P) : return Nil::Data::KeyCode::P;
    case(GLFW_KEY_Q) : return Nil::Data::KeyCode::Q;
    case(GLFW_KEY_R) : return Nil::Data::KeyCode::R;
    case(GLFW_KEY_S) : return Nil::Data::KeyCode::S;
    case(GLFW_KEY_T) : return Nil::Data::KeyCode::T;
    case(GLFW_KEY_U) : return Nil::Data::KeyCode::U;
    case(GLFW_KEY_V) : return Nil::Data::KeyCode::V;
    case(GLFW_KEY_W) : return Nil::Data::KeyCode::W;
    case(GLFW_KEY_X) : return Nil::Data::KeyCode::X;
    case(GLFW_KEY_Y) : return Nil::Data::KeyCode::Y;
    case(GLFW_KEY_Z) : return Nil::Data::KeyCode::Z;
    
    case(GLFW_KEY_1) : return Nil::Data::KeyCode::ONE;
    case(GLFW_KEY_2) : return Nil::Data::KeyCode::TWO;
    case(GLFW_KEY_3) : return Nil::Data::KeyCode::THREE;
    case(GLFW_KEY_4) : return Nil::Data::KeyCode::FOUR;
    case(GLFW_KEY_5) : return Nil::Data::KeyCode::FIVE;
    case(GLFW_KEY_6) : return Nil::Data::KeyCode::SIX;
    case(GLFW_KEY_7) : return Nil::Data::KeyCode::SEVEN;
    case(GLFW_KEY_8) : return Nil::Data::KeyCode::EIGHT;
    case(GLFW_KEY_9) : return Nil::Data::KeyCode::NINE;
    case(GLFW_KEY_0) : return Nil::Data::KeyCode::ZERO;
    
    case(GLFW_KEY_F1)  : return Nil::Data::KeyCode::F1;
    case(GLFW_KEY_F2)  : return Nil::Data::KeyCode::F2;
    case(GLFW_KEY_F3)  : return Nil::Data::KeyCode::F3;
    case(GLFW_KEY_F4)  : return Nil::Data::KeyCode::F4;
    case(GLFW_KEY_F5)  : return Nil::Data::KeyCode::F5;
    case(GLFW_KEY_F6)  : return Nil::Data::KeyCode::F6;
    case(GLFW_KEY_F7)  : return Nil::Data::KeyCode::F7;
    case(GLFW_KEY_F8)  : return Nil::Data::KeyCode::F8;
    case(GLFW_KEY_F9)  : return Nil::Data::KeyCode::F9;
    case(GLFW_KEY_F10) : return Nil::Data::KeyCode::F10;
    case(GLFW_KEY_F11) : return Nil::Data::KeyCode::F11;
    case(GLFW_KEY_F12) : return Nil::Data::KeyCode::F12;
    case(GLFW_KEY_F13) : return Nil::Data::KeyCode::F13;
    case(GLFW_KEY_F14) : return Nil::Data::KeyCode::F14;
    case(GLFW_KEY_F15) : return Nil::Data::KeyCode::F15;
    case(GLFW_KEY_F16) : return Nil::Data::KeyCode::F16;
    case(GLFW_KEY_F17) : return Nil::Data::KeyCode::F17;
    case(GLFW_KEY_F18) : return Nil::Data::KeyCode::F18;
    case(GLFW_KEY_F19) : return Nil::Data::KeyCode::F19;
    
    case(GLFW_KEY_SPACE)  : return Nil::Data::KeyCode::SPACE;
    case(GLFW_KEY_ESCAPE) : return Nil::Data::KeyCode::ESCAPE;
    
    default:
    
      return -1;
  }
}

} // anon ns


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
        
        #if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        Nil::Data::Window win_data = data[0];
        
        if(win_data.width == 0 || win_data.height == 0)
        {
          const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
          win_data.width = 2 * (mode->width / 3);
          win_data.height = 2 * (mode->height / 3);
          Nil::Data::set(self->window_node, win_data);
        }
        
        self->window = glfwCreateWindow(win_data.width, win_data.height, win_data.title, NULL, NULL);
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
        
        #ifndef NIMGUI
        ImGui_ImplGlfwGL3_Init(self->window, true);
        ImGui_ImplGlfwGL3_NewFrame();
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
      
      // Mouse
      {
        glfwSetInputMode(self->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        double x_pos, y_pos;
        glfwGetCursorPos(self->window, &x_pos, &y_pos);
        
        self->last_mouse_x = (float)x_pos;
        self->last_mouse_y = (float)y_pos;
        
        glfwSetCursorPosCallback(
          self->window,
          [](GLFWwindow *window, double x, double y)
          {
            Data *self = reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));
            LIB_ASSERT(self);
          
            const float this_mouse_x = (float)x;
            const float this_mouse_y = (float)y;
            
            self->delta_x = self->last_mouse_x - this_mouse_x;
            self->delta_y = self->last_mouse_y - this_mouse_y;
            
            self->last_mouse_x = this_mouse_x;
            self->last_mouse_y = this_mouse_y;
            
            size_t count = 0;
            Nil::Data::Mouse *mouse;
            Nil::Data::get(&count, &mouse);
            
            for(size_t i = 0; i < count; ++i)
            {
              if(mouse->id == 0)
              {
                mouse[i].delta[0] = math::to_int(self->delta_x);
                mouse[i].delta[1] = math::to_int(self->delta_y);
              }
            }
          }
        );
        
        glfwSetInputMode(self->window, GLFW_STICKY_KEYS, 1);
        glfwSetKeyCallback(
          self->window,
          [](GLFWwindow* window, int key, int scancode, int action, int mods)
          {
            Data *self = reinterpret_cast<Data*>(glfwGetWindowUserPointer(window));
            LIB_ASSERT(self);

            const uint32_t nil_key = glfw_key_to_nil(key);
            
            if(nil_key < Nil::Data::KeyCode::COUNT)
            {
              if(action != GLFW_REPEAT)
              {
                bool is_down = action != GLFW_RELEASE;
                const uint32_t input_action = is_down ? Nil::Data::KeyState::DOWN_ON_FRAME : Nil::Data::KeyState::UP_ON_FRAME;
                self->keys[nil_key] = input_action;
              }
            }
          }
        );
      }
    }
  }
  
  // Added Keyboards
  {
    size_t count = 0;
    Nil::Data::Keyboard *data = nullptr;
    Nil::Node *nodes;
    
    Nil::Data::events(Nil::Data::Event::ADDED, &count, &data, &nodes);
    
    for(size_t i = 0; i < count; ++i)
    {
      data[i].key_state = self->keys;
      data[i].key_count = Nil::Data::KeyCode::COUNT;
      
      Nil::Data::set(nodes[i], data[i]);
    }
  }
  
  // Quit?
  aspect.want_to_quit = glfwWindowShouldClose(self->window);
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
    // Update keyboard
    {
      for(uint32_t &key : self->keys)
      {
        if(key == Nil::Data::KeyState::DOWN_ON_FRAME)
        {
          key = Nil::Data::KeyState::DOWN;
        }
        else if(key == Nil::Data::KeyState::UP_ON_FRAME)
        {
          key = Nil::Data::KeyState::UP;
        }
      }
    }
  
    // Reset deltas
    {
      size_t count = 0;
      Nil::Data::Mouse *mouse;
      Nil::Data::get(&count, &mouse);

      for(size_t i = 0; i < count; ++i)
      {
        mouse[i].delta[0] = 0.f;
        mouse[i].delta[1] = 0.f;
      }
    }
    
    glfwPollEvents();
    
    #ifndef NIMGUI
    ImGui::Render();
    
    ImGui_ImplGlfwGL3_NewFrame();
//    ImGuizmo::BeginFrame();
    #endif
    
    glfwSwapBuffers(self->window);
  }
}


} // ns
} // ns