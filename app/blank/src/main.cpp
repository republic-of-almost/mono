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


/*
  Viewer Nodes.
*/
Nil::Node app;
Nil::Node camera;
Nil::Node scene;


/*
  Frame Tick
*/
void
app_tick()
{
  #ifdef __EMSCRIPTEN__
  nil_engine.run();
  #endif
  
  // Rotate the scene
  {
    uintptr_t user_data = scene.get_user_data();
    user_data += 7;
    scene.set_user_data(user_data);
    
//    printf("UD: %d\n", user_data);
    
    const float rot_angle = (float)user_data / 1000.f;
    
    const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, rot_angle);
    
    Nil::Data::Transform trans{};
    Nil::Data::get(scene, trans);
    math::quat_to_array(rot, trans.rotation);
    
    Nil::Data::set(scene, trans);
  }
}


/*
  Entry
*/
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
  
  
  // Scene
  {
    scene.set_name("Scene");
    scene.set_user_data(0);
  }
  
  // Load static assets
  {
//    const char *file = "/Users/PhilCK/Desktop/rep_of_a/assets/they_never_pay/mesh/static.obj";
//    
//    Nil::Resource::Scene::load(scene, file);
  }
  
  // Load bev cube
  {
    // Mesh Resource
    Nil::Resource::Model::load(
      Nil::Resource::directory("mesh/unit_bev_cube.obj")
    );
    
    Nil::Resource::Mesh mesh{};
    Nil::Resource::find_by_name("Unit_bev_cube", mesh);
    
    Nil::Resource::Material mat{};
    mat.color = 0xFF0000FF;
    Nil::Resource::load("Basic Mat", mat);
    
    Nil::Data::Renderable renderable{};
    renderable.mesh_id = mesh.id;
    renderable.material_id = mat.id;
    
    Nil::Data::set(scene, renderable);
    
    Nil::Data::set(scene, mesh.bounding_box);
  }
  
  // Camera
  {
    camera.set_name("Camera");
    Nil::Data::Camera cam{};
    cam.width              = 1.f;
    cam.height             = 1.f;
    cam.near_plane         = 0.1f;
    cam.far_plane          = 1000.f;
    cam.fov                = math::tau() * 0.125f;
    cam.clear_color_buffer = true;
    cam.clear_depth_buffer = true;
    
    Nil::Data::set(camera, cam);
    
    Nil::Data::Bounding_box target_bb{};
    Nil::Data::get(scene, target_bb);
    
    const math::vec3 a = math::vec3_init(target_bb.min);
    const math::vec3 b = math::vec3_init(target_bb.max);
    
    const float y = (math::abs(math::get_y(a)) + math::abs(math::get_y(b))) * 0.7f;
    const float z = (math::abs(math::get_z(a)) + math::abs(math::get_z(b))) * -4.f;
    
    Nil::Data::Transform trans{
      {0.f, y, z},
      {1.f, 1.f, 1.f},
      {0.f, 0.f, 0.f, 1.f},
    };
    
    Nil::Data::set(camera, trans);
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
