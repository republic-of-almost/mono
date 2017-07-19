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

#include <roa/roa.hpp>

#include <assert.h>
#include <stddef.h>
#include <string.h>


/*
  Viewer.
*/
Nil::Node scene;


ROA::Application app;
ROA::Object obj_camera;
ROA::Object obj_scene;


/*
  Entry
*/
int
main()
{
  lib::logging::set_output(lib::logging::out::console);
  
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
//    // Mesh Resource
//    Nil::Resource::Model::load(
//      Nil::Resource::directory("mesh/unit_bev_cube.obj")
//    );
//    
//    Nil::Resource::Mesh mesh{};
//    Nil::Resource::find_by_name("Unit_bev_cube", mesh);
//    
//    Nil::Resource::Material mat{};
//    mat.color = 0xFF0000FF;
//    Nil::Resource::load("Basic Mat", mat);
//    
//    Nil::Data::Renderable renderable{};
//    renderable.mesh_id = mesh.id;
//    renderable.material_id = mat.id;
//    
//    Nil::Data::set(scene, renderable);
//    
//    Nil::Data::set(scene, mesh.bounding_box);
  }
  
  {
    obj_scene.set_name("ROA_Scene");
  
    ROA::Model::load("mesh/unit_bev_cube.obj");
    ROA::Mesh mesh("Unit_bev_cube");
    
    ROA::Material mat("Basic Mat");
    mat.set_color(ROA::Color(0xFF0000FF));
    
    ROA::Renderable renderable;
    renderable.set_mesh(mesh);
    renderable.set_material(mat);
    
    obj_scene.set_renderable(renderable);
    obj_scene.set_bounding_box(mesh.get_bounding_box());
  }
  
  // Camera
  {
    obj_camera.set_name("ROA_Camera");
    
    ROA::Camera camera;
    obj_camera.set_camera(camera);
    
//    const ROA::Bounding_box bb = obj_scene.get_bounding_box();
    ROA::Bounding_box bb(ROA::Vector3(-0.5f, -0.5f, -0.5f), ROA::Vector3(0.5f, 0.5f, 0.5f));
    
//    Nil::Data::Bounding_box target_bb{};
//    Nil::Data::get(scene, target_bb);
    
    const math::vec3 a = math::vec3_init(bb.get_min().get_x(), bb.get_min().get_y(), bb.get_min().get_z());
    const math::vec3 b = math::vec3_init(bb.get_max().get_x(), bb.get_max().get_y(), bb.get_max().get_z());
    
    const float y = (math::abs(math::get_y(a)) + math::abs(math::get_y(b))) * 0.7f;
    const float z = (math::abs(math::get_z(a)) + math::abs(math::get_z(b))) * -4.f;
    
    ROA::Transform transform;
    transform.set_position(ROA::Vector3(0.f, y, z));
    transform.set_scale(ROA::Vector3(1.f, 1.f, 1.f));
    transform.set_rotation(ROA::Quaternion());
    
    obj_camera.set_transform(transform);
  }


  /*
    Run application
  */
  app.run(
    [](uintptr_t user_data)
    {
      /*
        Custom tick stuff.
      */
    
      static uint32_t spin = 0;
      spin += 7;
      
      const float rot_angle = (float)spin / 1000.f;
      
      const math::quat rot = math::quat_init_with_axis_angle(0, 1, 0, rot_angle);
      
      Nil::Data::Transform trans{};
      Nil::Data::get(scene, trans);
      math::quat_to_array(rot, trans.rotation);
      
      Nil::Data::set(scene, trans);
    
    },
    0
  );

  return 0;
}
