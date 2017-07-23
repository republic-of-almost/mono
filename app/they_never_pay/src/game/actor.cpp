#include <game/actor.hpp>
#include <data/data.hpp>
#include <nil/data/data.hpp>
#include <nil/resource/directory.hpp>
#include <math/math.hpp>
#include <lib/assert.hpp>
#include <lib/platform.hpp>
#include <lib/model.hpp>


namespace Game {


namespace {


inline void
think(ROA::Object node)
{
  Actor *actor = reinterpret_cast<Actor*>(node.get_user_data());
  LIB_ASSERT(actor);
  
  const float delta_time = 0.16f;
  
  if(ROA::Mouse::is_captured())
  {
    ROA::Point ms_delta = ROA::Mouse::get_delta();
  
    const float head_speed = 0.05f;
    const float final_speed = delta_time * head_speed;
    
    actor->accum_pitch += ms_delta.y * final_speed;
    actor->accum_yaw   -= ms_delta.x * final_speed;
    
    actor->accum_pitch = math::clamp(
      actor->accum_pitch,
      -math::quart_tau(),
      +math::quart_tau()
    );
  }

  // Actor trans
  {
    const math::quat yaw = math::quat_init_with_axis_angle(
      Game_data::get_world_up(),
      actor->accum_yaw
    );

    ROA::Transform trans = node.get_transform();
    trans.set_rotation(ROA::Quaternion(yaw.data));
  }
  
  // Head trans
  {
    const math::quat pitch = math::quat_init_with_axis_angle(
      Game_data::get_world_left(),
      actor->accum_pitch
    );

    ROA::Transform trans = actor->head.get_transform();
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
    }
    #endif

    const float move_speed = 0.5f * delta_time;

    ROA::Transform trans = node.get_transform();
    ROA::Transform head_trans = actor->head.get_transform();
    
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
    if((math::ray_test_triangles(ray, actor->nav_mesh, actor->nav_mesh_count, &distance)) && (math::abs(distance) < 2.f))
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
        actor->nav_mesh,
        actor->nav_mesh_count,
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
      if(math::ray_test_triangles(side_step_ray, actor->nav_mesh, actor->nav_mesh_count, &distance))
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
          actor->nav_mesh,
          actor->nav_mesh_count,
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
        if(math::ray_test_triangles(side_step_ray, actor->nav_mesh, actor->nav_mesh_count, &distance))
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
  
  for(uint32_t i = 0; i < actor->nav_mesh_count; ++i)
  {
    const size_t index = i * 9;
  
    Game_data::debug_line
    (
      math::vec3_init(&actor->nav_mesh[index + 0]),
      math::vec3_init(&actor->nav_mesh[index + 3]),
      math::vec3_init(1,0,0)
    );

    Game_data::debug_line
    (
      math::vec3_init(&actor->nav_mesh[index + 3]),
      math::vec3_init(&actor->nav_mesh[index + 6]),
      math::vec3_init(1,0,0)
    );

    Game_data::debug_line
    (
      math::vec3_init(&actor->nav_mesh[index + 6]),
      math::vec3_init(&actor->nav_mesh[index + 0]),
      math::vec3_init(1,0,0)
    );
  }

  // Ray Test
  {
    ROA::Transform trans = actor->head.get_transform();
    
    const ROA::Vector3 origin = trans.get_world_position();
    const ROA::Vector3 end = trans.get_world_position().add(trans.get_forward().scale(1000.f));
    
    ROA::Ray ray(origin, end);
    
    ROA::Collection collection = ROA::Query::bounding_boxes(ray, ROA::Ray_search::NEAREST);
    
    size_t size = collection.size();
    
    for(auto &col : collection)
    {
      int k = 0;
    }
  }

}


} // anon ns


void
setup(Actor *actor)
{
  LIB_ASSERT(actor);
  
  actor->height = 2.f;
  
  actor->entity.set_name("Actor");
  
  // Mouse Capture
  {
    Nil::Node mouse_node = Game_data::get_mouse();
  
    Nil::Data::Mouse ms{};
    Nil::Data::get(mouse_node, ms);
    
    ms.capture = true;
    Nil::Data::set(mouse_node, ms);
  }
  
  // Nav mesh
  {
    lib::model model = lib::model_import::load_obj_from_file(Nil::Resource::directory("mesh/nav_mesh.obj"));
    
    actor->nav_mesh = model.verts[0];
    actor->nav_mesh_count = model.triangle_count[0];
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
    
    actor->entity.set_transform(trans);
  }
  
  // Head
  {
    ROA::Object head;
    head.set_name("Head");
    head.set_parent(actor->entity);
    
    actor->head = head;
    
    // Head Trans
    {
      ROA::Transform trans;
      
      float pos[] = {0.f, math::g_ratio(), 0.f};
      float scale[] = {1.f, 1.f, 1.f};
      float rot[] = {0.f, 0.f, 0.f, 1.f};
      
      trans.set_position(ROA::Vector3(pos));
      trans.set_scale(ROA::Vector3(scale));
      trans.set_rotation(ROA::Quaternion(rot));
      
      actor->head.set_transform(trans);
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
//    Nil::Node body;
    ROA::Object body;
    body.set_name("Body");
    body.set_parent(actor->entity);
    
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
  
  // Callbacks
  {
    ROA::Logic logic;
    logic.update_func(think);
    
    actor->entity.set_user_data((uintptr_t)actor);
    actor->entity.set_logic(logic);
  }
  
  // Do one think to get actor into the right place
  {
    think(actor->entity);
  }
}


} // ns
