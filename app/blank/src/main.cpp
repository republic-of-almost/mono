#include <roa/roa.hpp>
#include <math/math.hpp>


ROA::Application app;
ROA::Object obj_camera(ROA::Data::CAMERA);
ROA::Object obj_scene(ROA::Data::RENDERABLE | ROA::Data::BOUNDING_BOX);


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
    
    const ROA::Mesh mesh = ROA::Resource::find<ROA::Mesh>("Unit_bev_cube");

    ROA::Material mat = ROA::Resource::create<ROA::Material>("Basic Mat");
    mat.set_color(ROA::Color(0xFFFF00FF));
    mat.load();

    const ROA::Renderable renderable(mesh, mat);

    obj_scene.set_data(renderable);
    obj_scene.set_data(mesh.get_bounding_box());
  }

  // Camera
  {
    obj_camera.set_name("ROA_Camera");

    const ROA::Bounding_box bb = obj_scene.get_data<ROA::Bounding_box>();

    const math::vec3 a = math::vec3_init(bb.get_min().get_x(), bb.get_min().get_y(), bb.get_min().get_z());
    const math::vec3 b = math::vec3_init(bb.get_max().get_x(), bb.get_max().get_y(), bb.get_max().get_z());

    const float y = (math::abs(math::get_y(a)) + math::abs(math::get_y(b))) * 0.7f;
    const float z = (math::abs(math::get_z(a)) + math::abs(math::get_z(b))) * 4.f;

    const ROA::Transform transform(
      ROA::Vector3(0.f, y, z),
      ROA::Vector3(1.f, 1.f, 1.f),
      ROA::Quaternion()
    );

    obj_camera.set_data(transform);
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

      static float spin = 0.f;
      constexpr float speed = 0.15f;
      const float delta_speed = speed * ROA::Time::get_delta_time();

      spin += delta_speed;

      ROA::Material mat = ROA::Resource::create<ROA::Material>("Basic Mat");
      mat.set_color(ROA::Color(0xFF00FFFF));

      const float rot_angle = spin;
      const ROA::Quaternion rot(ROA::Vector3(1.f, 0.f, 0.f), rot_angle);

      ROA::Transform trans = obj->get_data<ROA::Transform>();
      trans.set_rotation(rot);
    },
    (uintptr_t)&obj_scene
  );

  return 0;
}
