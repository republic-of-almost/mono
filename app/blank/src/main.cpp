#include <roa/roa.hpp>
#include <math/math.hpp>


ROA::Application app;
ROA::Object obj_camera;
ROA::Object obj_scene;


int
main()
{
  /*
    Setup Objects
  */
  
  // Scene
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
      ROA::Object *obj = reinterpret_cast<ROA::Object*>(user_data);
    
      static uint32_t spin = 0;
      spin += 7;
      
      const float rot_angle = (float)spin / 1000.f;
      const ROA::Quaternion rot(ROA::Vector3(0.f, 1.f, 0.f), rot_angle);
      
      ROA::Transform trans = obj->get_transform();
      trans.set_rotation(rot);
    },
    (uintptr_t)&obj_scene
  );

  return 0;
}
