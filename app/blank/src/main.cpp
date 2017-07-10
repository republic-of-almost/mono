#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/data/data.hpp>
#include <nil/resource/resource.hpp>
#include <aspect/aspects.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <lib/logging.hpp>
#include <lib/string.hpp>

#include <assert.h>
#include <stddef.h>
#include <string.h>


Nil::Node app;
Nil::Node camera;
Nil::Node scene;


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
  
  // Basic App
  {
    app.set_name("App");
    Nil::Node node = app;
    
    Nil::Data::Window win{};
    constexpr char title[] = "Viewer";
    strcat(win.title, title);
    
    Nil::Data::set(node, win);
    Nil::Data::set(node, Nil::Data::Mouse{});
    Nil::Data::set(node, Nil::Data::Keyboard{});
  }
  
  // Camera
  {
    camera.set_name("Camera");
    Nil::Data::Camera cam{};
    cam.width = 1.f;
    cam.height = 1.f;
    cam.near_plane = 0.1f;
    cam.far_plane = 1000.f;
    cam.fov = math::tau() * 0.125;
    cam.clear_color_buffer = true;
    cam.clear_depth_buffer = true;
    
    Nil::Data::set(camera, cam);
  }
  
    // Load static assets
  {
    scene.set_name("Scene");
    const char *file = "/Users/PhilCK/Desktop/rep_of_a/assets/they_never_pay/mesh/static.obj";
    
    Nil::Resource::Scene::load(scene, file);
  }

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
