#include <roa/roa.hpp>
#include <math/math.hpp>



int
main()
{
  // -------------------------------------------------------- [ Load Assets ] --
  
  ROA::Model::load("mesh/unit_bev_cube.obj");
  
  // -------------------------------------------------------- [ Game Assets ] --
  
  ROA::Material mat  = ROA::Resource::create<ROA::Material>("Basic Mat");
  ROA::Shader   shd  = ROA::Resource::create<ROA::Shader>("world_pos");
  ROA::Mesh     mesh = ROA::Resource::find<ROA::Mesh>("Unit_bev_cube");

  // ------------------------------------------------------- [ Game Objects ] --
  
  ROA::Application app;
  ROA::Object      obj_camera;
  ROA::Object      obj_scene;

  // ------------------------------------------------------- [ Setup Assets ] --
  
  {
    // Shader //
    shd.set_vertex_shader_code(
      R"GLSL(
        #version 100

        attribute mediump vec3 vs_in_position;
        attribute mediump vec3 vs_in_normal;
        attribute mediump vec2 vs_in_texture_coords;

        uniform mediump mat4 uni_wvp;

        varying mediump vec2 ps_in_texture_coords;
        varying mediump vec3 ps_in_color;

        void
        main()
        {
          gl_Position          = uni_wvp * vec4(vs_in_position, 1.0);
          ps_in_texture_coords = vs_in_texture_coords;
          ps_in_color          = vs_in_normal;
        }
      )GLSL"
    );
    
    shd.set_fragment_shader_code(
      R"GLSL(
        #version 100

        varying mediump vec2 ps_in_texture_coords;
        varying mediump vec3 ps_in_color;

        uniform mediump vec4 uni_color;

        void
        main()
        {
          gl_FragColor = vec4(1.f,0.f,0.f,1.f);
        }
      )GLSL"
    );
    
    shd.load();
    
    // Material //
    mat.set_color(ROA::Color(0xFFFF00FF));
    mat.set_shader(shd);
    mat.load();
  }

  // ------------------------------------------------------ [ Setup Objects ] --
  
  {
    // Scene
    {
      obj_scene.set_name("ROA_Scene");
      obj_scene.add_data<ROA::Renderable>();
      obj_scene.add_data<ROA::Bounding_box>();

      const ROA::Renderable renderable(mesh, mat);

      obj_scene.set_data(renderable);
      obj_scene.set_data(mesh.get_bounding_box());
    }

    // Camera
    {
      obj_camera.set_name("ROA_Camera");
      
      obj_camera.add_data<ROA::Camera>();

      const ROA::Bounding_box bb = obj_scene.get_data<ROA::Bounding_box>();

      const math::vec3 a = math::vec3_init(bb.get_min().get_data());
      const math::vec3 b = math::vec3_init(bb.get_max().get_data());

      const float y = (math::abs(math::get_y(a)) + math::abs(math::get_y(b))) * 0.7f;
      const float z = (math::abs(math::get_z(a)) + math::abs(math::get_z(b))) * 4.f;

      const ROA::Transform transform(
        ROA::Vector3(0.f, y, z),
        ROA::Vector3(1.f, 1.f, 1.f),
        ROA::Quaternion()
      );

      obj_camera.set_data(transform);
    }
  }

  // -------------------------------------------------------- [ Application ] --
  
  app.run(
    [](uintptr_t user_data)
    {
      ROA::Object *obj = reinterpret_cast<ROA::Object*>(user_data);
      ROA_ASSERT(obj);
      ROA_ASSERT(obj->is_valid());

      constexpr float speed   = 0.15f;
      static float spin       = 0.f;
      const float dt = ROA::Time::get_delta_time();
      const float delta_speed = speed * dt;

      spin += delta_speed;

      const float rot_angle = spin;
      const ROA::Quaternion rot(ROA::Vector3(1.f, 0.f, 0.f), rot_angle);

      ROA::Transform trans = obj->get_data<ROA::Transform>();
      trans.set_rotation(rot);
    },
    (uintptr_t)&obj_scene
  );

  return 0;
}
