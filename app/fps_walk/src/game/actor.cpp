#include <game/actor.hpp>
#include <game/data.hpp>
#include <nil/data/data.hpp>
#include <nil/resource/resource.hpp>
#include <math/geometry/ray.hpp>
#include <math/general/general.hpp>
#include <math/quat/quat.hpp>
#include <lib/assert.hpp>
#include <lib/platform.hpp>


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
    const ROA::Point ms_delta = ROA::Mouse::get_delta();
    
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
    Nil::Data::Keyboard kb{};
    Nil::Data::get(Game_data::get_keyboard(), kb);

    float z_move = 0.f;
    float x_move = 0.f;
    
    #ifndef LIB_PLATFORM_WEB
    if(kb.key_state)
    {
      if(kb.key_state[Nil::Data::KeyCode::W] == Nil::Data::KeyState::DOWN)
      {
        z_move += 1.f;
      }
      if(kb.key_state[Nil::Data::KeyCode::S] == Nil::Data::KeyState::DOWN)
      {
        z_move -= 1.f;
      }
      if(kb.key_state[Nil::Data::KeyCode::A] == Nil::Data::KeyState::DOWN)
      {
        x_move += 1.f;
      }
      if(kb.key_state[Nil::Data::KeyCode::D] == Nil::Data::KeyState::DOWN)
      {
        x_move -= 1.f;
      }
      
      // Toggle mouse capture //
      #ifndef LIB_PLATFORM_WEB
      if(kb.key_state[Nil::Data::KeyCode::ESCAPE] == Nil::Data::KeyState::UP_ON_FRAME)
      {
        const bool capture = !ROA::Mouse::is_captured();
        ROA::Mouse::set_captured(capture);
      }
      #endif
    }
    #endif

    const float move_speed = 0.5f * delta_time;
    
    ROA::Transform trans = node.get_transform();
    
    math::vec3 next_step = math::vec3_init(
      trans.get_position().get_x(),
      trans.get_position().get_y(),
      trans.get_position().get_z()
    );
    
    math::vec3 move_dir  = math::vec3_zero();

    const math::vec3 local_movement = math::vec3_init(x_move, 0.f, z_move);
    
    /*
      If actor is trying to move, find out what the next desired step should be.
    */
    if(math::vec3_length(local_movement) > math::epsilon())
    {
      const math::quat rot = math::quat_init(
        trans.get_rotation().get_x(),
        trans.get_rotation().get_y(),
        trans.get_rotation().get_z(),
        trans.get_rotation().get_w()
      );
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
    constexpr size_t tri_count = sizeof(Actor::nav_mesh) / (sizeof(float)) / 9;
    
    float distance = 0.f;
    if(math::ray_test_triangles(ray, actor->nav_mesh, tri_count, &distance))
    {
      math::vec3 scale = math::vec3_scale(math::ray_direction(ray), distance);
      math::vec3 hit = math::vec3_add(ray.start, scale);
    
      const math::vec3 height = math::vec3_init(0, actor->height, 0);
      const math::vec3 pos    = math::vec3_add(hit, height);
      
      trans.set_position(ROA::Vector3(pos.data));
    }
    
    /*
      Else try and find an alternative step.
    */
    else if(math::vec3_length(move_dir))
    {
      math::vec3 start;
      math::vec3 end;
      
      math::ray_test_closest_edge(
        actor->nav_mesh,
        tri_count,
        next_step,
        start,
        end
      );
      
      const math::vec3 norm_edge = math::vec3_normalize(
        math::vec3_subtract(end, start)
      );
      
      const float dot = math::vec3_dot(norm_edge, math::vec3_normalize(move_dir));
      
      const math::vec3 curr_pos = math::vec3_init(
        trans.get_position().get_x(),
        trans.get_position().get_y(),
        trans.get_position().get_z()
      );
      
      const math::vec3 side_step         = math::vec3_add(curr_pos, math::vec3_scale(norm_edge, dot * move_speed));
      const math::vec3 side_step_ray_end = math::vec3_add(side_step, math::vec3_init(0.f, -10000.f, 0.f));
      const math::ray side_step_ray      = math::ray_init(side_step, side_step_ray_end);
      
      distance = 0.f;
      if(math::ray_test_triangles(side_step_ray, actor->nav_mesh, tri_count, &distance))
      {
        const math::vec3 scale  = math::vec3_scale(math::ray_direction(side_step_ray), distance);
        const math::vec3 hit    = math::vec3_add(side_step_ray.start, scale);
        const math::vec3 height = math::vec3_init(0, actor->height, 0);
        const math::vec3 pos    = math::vec3_add(hit, height);

        trans.set_position(ROA::Vector3(
          pos.data
        ));
      }
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
  
  actor->entity.set_user_data((uintptr_t)actor);
  
  // Mouse Capture
  {
    Nil::Node mouse_node = Game_data::get_mouse();
  
    Nil::Data::Mouse ms{};
    Nil::Data::get(mouse_node, ms);
    
    ms.capture = true;
    Nil::Data::set(mouse_node, ms);
  }
  
  // Nav Mesh
  {
    constexpr float scale = 7.f;
    
    constexpr float mesh[] = {
      +scale, 0.f, -scale,
      -scale, 0.f, +scale,
      +scale, 0.f, +scale,

      -scale, 0.f, +scale,
      +scale, 0.f, -scale,
      -scale, 0.f, -scale,
    };
    
    memcpy(actor->nav_mesh, mesh, sizeof(actor->nav_mesh));
  }
  
  // Main Entity
  {
    float pos[] = {0.f, 0.f, 0.f};
    float scale[] = {1.f, 1.f, 1.f};
    float rot[] = {0.f, 0.f, 0.f, 1.f};
    
    ROA::Transform trans = actor->entity.get_transform();
    trans.set_position(ROA::Vector3(pos));
    trans.set_scale(ROA::Vector3(scale));
    trans.set_rotation(ROA::Quaternion(rot));
  }
  
  // Head
  {
    ROA::Object head;
    head.set_name("Head");
    head.set_parent(actor->entity);
    
    actor->head = head;
    
    // Head Trans
    {
      float pos[] = {0.f, 1 + math::g_ratio(), 0.f};
      float scale[] = {1.f, 1.f, 1.f};
      float rot[] = {0.f, 0.f, 0.f, 1.f};
      
      ROA::Transform trans = head.get_transform();
      trans.set_position(ROA::Vector3(pos));
      trans.set_scale(ROA::Vector3(scale));
      trans.set_rotation(ROA::Quaternion(rot));
    }
    
    // Camera
    {
      ROA::Object camera;
      camera.set_name("Camera");
      camera.set_parent(head);
      
      ROA::Camera cam_data;
      cam_data.set_field_of_view(math::tau() * 0.2f);
      cam_data.set_clear_color(ROA::Color(0xFFFF00FF));
      camera.set_camera(cam_data);
    } // cam
  } // head
  
  // Body
  {
    ROA::Object body;
    body.set_name("Body");
    body.set_parent(actor->entity);
    
    float pos[] = {0.f, 0.f, 0.f};
    float scale[] = {1.f, 1 + math::g_ratio(), 1.f};
    float rot[] = {0.f, 0.f, 0.f, 1.f};

    ROA::Transform trans = body.get_transform();
    trans.set_position(ROA::Vector3(pos));
    trans.set_scale(ROA::Vector3(scale));
    trans.set_rotation(ROA::Quaternion(rot));
  }
  
  // Callbacks
  {
    ROA::Logic logic;
    
    logic.update_func(think);
    actor->entity.set_logic(logic);
  }
  
  // Do one think to get actor into the right place
  {
    think(actor->entity);
  }
}


} // ns
