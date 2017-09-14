#include <game/actor_kinematic.hpp>
#include <data/data.hpp>
#include <nil/data/data.hpp>
#include <nil/resource/directory.hpp>
#include <math/math.hpp>
#include <lib/assert.hpp>
#include <lib/platform.hpp>
#include <lib/model.hpp>


namespace Game {


void
Actor_kinematic::on_think()
{
  const float delta_time = 0.16f;
  
  ROA::Object entity = get_object();
  
  if(ROA::Mouse::is_captured())
  {
    ROA::Point ms_delta = ROA::Mouse::get_delta();
  
    const float head_speed = 0.05f;
    const float final_speed = delta_time * head_speed;
    
    this->accum_pitch -= ms_delta.y * final_speed;
    this->accum_yaw   += ms_delta.x * final_speed;
    
    this->accum_pitch = math::clamp(
      this->accum_pitch,
      -math::quart_tau(),
      +math::quart_tau()
    );
  }

  // Actor trans
  {
    const math::quat yaw = math::quat_init_with_axis_angle(
      Game_data::get_world_up(),
      this->accum_yaw
    );

    ROA::Transform trans = entity.get_transform();
    trans.set_rotation(ROA::Quaternion(yaw.data));
  }
  
  // Head trans
  {
    const math::quat pitch = math::quat_init_with_axis_angle(
      Game_data::get_world_left(),
      this->accum_pitch
    );

    ROA::Transform trans = this->head.get_transform();
    trans.set_rotation(ROA::Quaternion(pitch.data));
  }
  
  // Movement
  {
    float z_move = 0.f;
    float x_move = 0.f;
    
    #ifndef LIB_PLATFORM_WEB
    {
      if(ROA::Keyboard::key_state(ROA::KeyCode::W) == ROA::KeyState::DOWN)
      {
        player.play();
        z_move += 1.f;
      }
      if(ROA::Keyboard::key_state(ROA::KeyCode::S) == ROA::KeyState::DOWN)
      {
        z_move -= 1.f;
      }
      if(ROA::Keyboard::key_state(ROA::KeyCode::A) == ROA::KeyState::DOWN)
      {
        x_move += 1.f;
      }
      if(ROA::Keyboard::key_state(ROA::KeyCode::D) == ROA::KeyState::DOWN)
      {
        x_move -= 1.f;
      }
    
    
      // Toggle mouse capture //
      #ifndef LIB_PLATFORM_WEB
      if(ROA::Keyboard::key_state(ROA::KeyCode::ESCAPE) == ROA::KeyState::UP_ON_FRAME)
      {
        const bool capture = !ROA::Mouse::is_captured();
        ROA::Mouse::set_captured(capture);
      }
      #endif
      
      // Create query
      if(ROA::Keyboard::key_state(ROA::KeyCode::SPACE) == ROA::KeyState::UP_ON_FRAME)
      {
        ROA::Transform trans = this->head.get_transform();
      
        ROA::Ray ray(trans.get_world_position(), trans.get_world_forward().scale(10000));
      
        ROA::Collection collection = ROA::Query::bounding_boxes(ray, ROA::Ray_search::NEAREST);
        
        for(auto &c : collection)
        {
          if(strcmp(c.get_name(), "Dyn_football") == 0)
          {
            ROA::Logic logic = c.get_logic();
            logic.send_message(1, 1); 
          }
        }
      }
    }
    #endif

    const float move_speed = 0.5f * delta_time;

    ROA::Transform trans = entity.get_transform();
    ROA::Transform head_trans = this->head.get_transform();
    
    math::vec3 height = math::vec3_init(head_trans.get_position().get_data());
    math::vec3 pos = math::vec3_init(trans.get_position().get_data());
    
    const math::vec3 curr_step = math::vec3_add(pos, height);
    math::vec3 next_step = curr_step;
    math::vec3 move_dir  = math::vec3_zero();

    const math::vec3 local_movement = math::vec3_init(x_move, 0.f, z_move);
    
    /*
      If actor is trying to move, find out what the next desired step should be.
    */
    if(math::vec3_length(local_movement) > math::epsilon())
    {
      const math::quat rot           = math::quat_init(trans.get_rotation().get_data());
      const math::vec3 norm_movement = math::vec3_normalize(local_movement);
      
      const math::vec3 ent_fwd = math::quat_rotate_point(rot, Game_data::get_world_fwd());
      const float scale_fwd    = move_speed * math::get_z(norm_movement);
      const math::vec3 fwd     = math::vec3_scale(ent_fwd, scale_fwd);
      
      const math::vec3 ent_left = math::quat_rotate_point(rot, Game_data::get_world_left());
      const float scale_left    = move_speed * math::get_x(norm_movement);
      const math::vec3 left     = math::vec3_scale(ent_left, scale_left);
      
      move_dir  = math::vec3_add(fwd, left);
      next_step = math::vec3_add(next_step, move_dir);
    }
    
    /*
      Desired Step
    */
    const math::vec3 end_ray   = math::vec3_add(math::vec3_init(0, -1000, 0), next_step);
    const math::ray ray        = math::ray_init(next_step, end_ray);
    
    /*
      First check the nav mesh to see if its a valid step.
    */
    //constexpr size_t tri_count = sizeof(Actor::nav_mesh) / (sizeof(float)) / 9;
    
    float distance = 0.f;
    if((math::ray_test_triangles(ray, this->nav_mesh, this->nav_mesh_count, &distance)) && (math::abs(distance) < 5.f))
    {
      math::vec3 scale = math::vec3_scale(math::ray_direction(ray), distance);
      math::vec3 hit = math::vec3_add(ray.start, scale);

      const math::vec3 pos = hit;
      
      trans.set_position(ROA::Vector3(pos.data));
    }
    
    /*
      Else try and find an alternative step.
    */
    else if(math::vec3_length(move_dir) > math::epsilon())
    {
      math::vec3 curr_step_edge[2];
      
      math::ray_test_closest_edge(
        this->nav_mesh,
        this->nav_mesh_count,
        curr_step,
        curr_step_edge[0],
        curr_step_edge[1]
      );
      
      Game_data::debug_line(curr_step_edge[0], curr_step_edge[1], math::vec3_init(1,1,0));
      
      const math::vec3 norm_edge = math::vec3_normalize(
        math::vec3_subtract(curr_step_edge[0], curr_step_edge[1])
      );
      
      const float dot = math::vec3_dot(norm_edge, math::vec3_normalize(move_dir));
      
      const math::vec3 curr_pos = math::vec3_init(trans.get_position().get_data());
      
      const math::vec3 side_step         = math::vec3_add(curr_step, math::vec3_scale(norm_edge, dot * move_speed));
      const math::vec3 side_step_ray_end = math::vec3_add(side_step, math::vec3_init(0.f, -10000.f, 0.f));
      const math::ray side_step_ray      = math::ray_init(side_step, side_step_ray_end);
      
      distance = 0.f;
      if(math::ray_test_triangles(side_step_ray, this->nav_mesh, this->nav_mesh_count, &distance))
      {
        const math::vec3 ray_dir = math::ray_direction(side_step_ray);
        const math::vec3 scale   = math::vec3_scale(ray_dir, distance);
        const math::vec3 hit     = math::vec3_add(side_step_ray.start, scale);
        const math::vec3 pos     = hit;
        
        trans.set_position(ROA::Vector3(pos.data));
      }
      else
      {
        math::vec3 next_step_edge[2];
      
        math::ray_test_closest_edge(
          this->nav_mesh,
          this->nav_mesh_count,
          next_step,
          next_step_edge[0],
          next_step_edge[1]
        );
        
        Game_data::debug_line(next_step_edge[0], next_step_edge[1], math::vec3_init(1,1,0));
        
        const math::vec3 norm_edge = math::vec3_normalize(
          math::vec3_subtract(next_step_edge[0], next_step_edge[1])
        );
        
        const float dot = math::vec3_dot(norm_edge, math::vec3_normalize(move_dir));
        
        const math::vec3 curr_pos = math::vec3_init(trans.get_position().get_data());
        
        const math::vec3 side_step         = math::vec3_add(curr_pos, math::vec3_scale(norm_edge, dot * move_speed));
        const math::vec3 side_step_ray_end = math::vec3_add(side_step, math::vec3_init(0.f, -10000.f, 0.f));
        const math::ray side_step_ray      = math::ray_init(side_step, side_step_ray_end);
        
        distance = 0.f;
        if(math::ray_test_triangles(side_step_ray, this->nav_mesh, this->nav_mesh_count, &distance))
        {
          const math::vec3 ray_dir = math::ray_direction(side_step_ray);
          const math::vec3 scale   = math::vec3_scale(ray_dir, distance);
          const math::vec3 hit     = math::vec3_add(side_step_ray.start, scale);
          const math::vec3 pos     = hit;
          
          trans.set_position(ROA::Vector3(pos.data));
        }
      }
    }
  }
  
//  constexpr size_t tri_count = sizeof(Actor::nav_mesh) / (sizeof(float)) / 9;
  
  for(uint32_t i = 0; i < this->nav_mesh_count; ++i)
  {
    const size_t index = i * 9;
  
    Game_data::debug_line
    (
      math::vec3_init(&this->nav_mesh[index + 0]),
      math::vec3_init(&this->nav_mesh[index + 3]),
      math::vec3_init(1,0,0)
    );

    Game_data::debug_line
    (
      math::vec3_init(&this->nav_mesh[index + 3]),
      math::vec3_init(&this->nav_mesh[index + 6]),
      math::vec3_init(1,0,0)
    );

    Game_data::debug_line
    (
      math::vec3_init(&this->nav_mesh[index + 6]),
      math::vec3_init(&this->nav_mesh[index + 0]),
      math::vec3_init(1,0,0)
    );
  }
}


//} // anon ns


void
Actor_kinematic::on_start()
{
  this->height = 2.f;
  
  ROA::Object entity = get_object();
  
//  // Nav mesh
//  {
//    lib::model model = lib::model_import::load_obj_from_file(
//      Nil::Resource::directory("mesh/nav_mesh.obj")
//    );
//    
//    float foo[32]{};
//    memcpy(foo, model.verts[0], sizeof(foo));
//
//    if(model.mesh_count > 0) 
//    {
//      this->nav_mesh = model.verts[0];
//      this->nav_mesh_count = model.triangle_count[0];
//    }
//  }

  // Audio Test
  {
    ROA::Audio_source audio_src("/Users/PhilCK/Desktop/test.ogg");
    
    player.set_source(audio_src);
    
    entity.set_audio_player(player);
  }
  
  // Main Entity
  {
    ROA::Transform trans;
    
    float pos[] = {1.f, 0.f, 0.f};
    float scale[] = {1.f, 1.f, 1.f};
    float rot[] = {0.f, 0.f, 0.f, 1.f};
    
    trans.set_position(ROA::Vector3(pos));
    trans.set_scale(ROA::Vector3(scale));
    trans.set_rotation(ROA::Quaternion(rot));
    
    entity.set_transform(trans);
  }
  
  // Head
  {
    this->head = ROA::Object{};
    head.set_name("Head");
    head.set_parent(entity);
    
    
    // Head Trans
    {
      ROA::Transform trans;
      
      float pos[]   = {0.f, math::g_ratio() * 2, 0.f};
      float scale[] = {1.f, 1.f, 1.f};
      float rot[]   = {0.f, 0.f, 0.f, 1.f};
      
      trans.set_position(ROA::Vector3(pos));
      trans.set_scale(ROA::Vector3(scale));
      trans.set_rotation(ROA::Quaternion(rot));
      
      this->head.set_transform(trans);
    }
    
    // Camera
    {
      ROA::Object camera;
      camera.set_name("Camera");
      camera.set_parent(head);
    
      // Transform
      {
        ROA::Transform trans;
        
        float rot[] = {0.f, 0.f, 0.f, 1.f};
        trans.set_rotation(ROA::Quaternion(rot));
        
        camera.set_transform(trans);
      }
      
      // Camera
      {
        ROA::Camera cam_data;
        cam_data.set_field_of_view(math::tau() * 0.12f);
        cam_data.set_clear_color(ROA::Color(0x111122FF));
        
        camera.set_camera(cam_data);
      }
      
      
    } // cam
  } // head
  
  // Body
  {
    this->body = ROA::Object{};
    body.set_name("Body");
    body.set_parent(entity);
    
    // Body Transform
    {
      float pos[] = {0.f, 0.f, 0.f};
      float scale[] = {1.f, math::g_ratio(), 1.f};
      float rot[] = {0.f, 0.f, 0.f, 1.f};

      ROA::Transform trans;
      trans.set_position(ROA::Vector3(pos));
      trans.set_scale(ROA::Vector3(scale));
      trans.set_rotation(ROA::Quaternion(rot));
      
      body.set_transform(trans);
    }
  }
}


} // ns