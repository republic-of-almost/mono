//#include <GLFW/glfw3.h>
//
//int main(void)
//{
//    GLFWwindow* window;
//
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    /* Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//        /* Render here */
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//
//        /* Poll for and process events */
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}

#include <nil/nil.hpp>
#include <aspect/aspects.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <lib/logging.hpp>
#include <lib/string.hpp>

#include <game/static_objects.hpp>
#include <game/actor.hpp>

#include <data/data.hpp>

#include <assert.h>
#include <stddef.h>
#include <string.h>


void
app_tick()
{
  #ifdef __EMSCRIPTEN__
  nil_engine.run();
  #endif
}


int
main()
{
  lib::logging::set_output(lib::logging::out::console);
  
  Nil::Engine nil_engine;
  Nil_ext::load_aspects(nil_engine);
  
  Game_data::setup();
  Game_data::load_assets();

  Game::Actor actor;
  Game::setup(&actor);
  
  Game::Static_objects objs;
  Game::setup(&objs);
  
  /*
    Run Game
  */
  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(app_tick, -1, 1);
  #else
  while(nil_engine.run())
  {
    app_tick();
  }
  #endif

  return 0;
}
