#include <roa/roa.hpp>
#include <math/math.hpp>
#include <game_code/freecam.hpp>
#include <game_code/controller.hpp>


int
main()
{
  /* ----------------------------------------------------- [ Load Assets ] -- */
  
  ROA::Model::load("mesh/unit_plane.obj");
  
  /* ----------------------------------------------------- [ Game Assets ] -- */
  
  ROA::Material mat  = ROA::Resource::create<ROA::Material>("Basic Mat");
  ROA::Shader   shd  = ROA::Resource::create<ROA::Shader>("world_pos");
  ROA::Mesh     mesh = ROA::Resource::find<ROA::Mesh>("Unit_plane");

  /* ---------------------------------------------------- [ Game Objects ] -- */
  
  roa_application_init();

  /* ---------------------------------------------------- [ Setup Assets ] -- */
  
  {
    // Material //
    mat.set_color(ROA::Color(0xFF00FFFF));
    mat.load();
  }
  
  /* --------------------------------------------------- [ Setup Objects ] -- */
  
  struct World_state
  {
    Free_cam    free_cam{};
    Controller  controllers[1]{};
    ROA::Object obj_camera;
    ROA::Object obj_scene;
    
  } world;
  
  {
    // Scene
    {
      world.obj_scene.set_name("ROA_Scene");
      world.obj_scene.add_data<ROA::Renderable>();
      world.obj_scene.add_data<ROA::Bounding_box>();

      const ROA::Renderable renderable(mesh, mat);

      world.obj_scene.set_data(renderable);
      world.obj_scene.set_data(mesh.get_bounding_box());
      
      ROA::Transform trans = world.obj_scene.get_data<ROA::Transform>();
      trans.set_scale(ROA::Vector3(10.f, 1.f, 10.f));
    }
    
    // Light
    {
      ROA::Object light;
      light.add_data<ROA::Light>();
      
      ROA::Light light_data = light.get_data<ROA::Light>();
      light_data.set_linear_atten(0.01f);
      light_data.set_type(ROA::Light_type::POINT);
      
      light.set_parent(world.obj_scene);
    }
    
    // Camera
    {
      world.obj_camera.set_name("ROA_Camera");
      
      world.obj_camera.add_data<ROA::Camera>();
      ROA::Camera cam = world.obj_camera.get_data<ROA::Camera>();
      cam.set_clear_color(ROA::Color(0xFF00FFFF));

      const ROA::Bounding_box bb = world.obj_scene.get_data<ROA::Bounding_box>();
      
      const math::vec3 a = math::vec3_init(bb.get_min().get_data());
      const math::vec3 b = math::vec3_init(bb.get_max().get_data());
      
      const float y = (math::abs(math::get_y(a)) + math::abs(math::get_y(b))) * 0.7f;
      const float z = (math::abs(math::get_z(a)) + math::abs(math::get_z(b))) * 4.f;
      
      const ROA::Transform transform(
        ROA::Vector3(0.f, 1.f, z),
        ROA::Vector3(1.f, 1.f, 1.f),
        ROA::Quaternion()
      );
      
      world.obj_camera.set_data(transform);
    }
  }

  /* ----------------------------------------------------- [ Application ] -- */
  
  roa_application_run_with_tick(
    (uintptr_t)&world,
    [](uintptr_t user_data)
    {
      ROA::Mouse::set_captured(true); // Bug if you capture before window is created it doens't get set.
      
      World_state *world = (World_state*)user_data;
    
      controller_update(world->controllers, 1);
      free_cam_apply_controller(&world->free_cam, world->controllers, 1);
      free_cam_update(&world->free_cam, &world->obj_camera, 1);
      
    }
  );

  return 0;
}
