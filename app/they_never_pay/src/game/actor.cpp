#include <game/actor.hpp>
#include <data/data.hpp>
#include <nil/data/logic.hpp>
#include <nil/data/camera.hpp>
#include <nil/data/mouse.hpp>
#include <nil/data/keyboard.hpp>
#include <nil/data/renderable.hpp>
#include <nil/data/transform.hpp>
#include <math/geometry/ray.hpp>
#include <math/general/general.hpp>
#include <math/quat/quat.hpp>
#include <lib/assert.hpp>
#include <lib/platform.hpp>
#include <lib/model.hpp>


namespace Game {


namespace {


inline void
think(Nil::Node node, uintptr_t user_data)
{
  Actor *actor = reinterpret_cast<Actor*>(user_data);
  LIB_ASSERT(actor);
  
  Nil::Node ms_node = Game_data::get_mouse();
  Nil::Data::Mouse ms{};
  Nil::Data::get(ms_node, ms);
  
  const float delta_time = 0.16f;
  
  if(ms.capture)
  {
    const float head_speed = 0.05f;
    const float final_speed = delta_time * head_speed;
    
    actor->accum_pitch += ms.delta[1] * final_speed;
    actor->accum_yaw   -= ms.delta[0] * final_speed;
    
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
    
    Nil::Data::Transform trans{};
    Nil::Data::get(node, trans);
    
    memcpy(trans.rotation, yaw.data, sizeof(trans.rotation));

    Nil::Data::set(node, trans);
  }
  
  // Head trans
  {
    const math::quat pitch = math::quat_init_with_axis_angle(
      Game_data::get_world_left(),
      actor->accum_pitch
    );
    
    Nil::Data::Transform trans{};
    Nil::Data::get(actor->head, trans);
    
    memcpy(trans.rotation, pitch.data, sizeof(trans.rotation));
    
    Nil::Data::set(actor->head, trans);
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
        ms.capture = !ms.capture;
        Nil::Data::set(ms_node, ms);
      }
      #endif
    }
    #endif

    const float move_speed = 0.5f * delta_time;
    
    Nil::Data::Transform trans{};
    Nil::Data::get(node, trans);
    
    const math::vec3 curr_step = math::vec3_init_with_array(trans.position);
    math::vec3 next_step = curr_step;
    math::vec3 move_dir  = math::vec3_zero();

    const math::vec3 local_movement = math::vec3_init(x_move, 0.f, z_move);
    
    /*
      If actor is trying to move, find out what the next desired step should be.
    */
    if(math::vec3_length(local_movement) > math::epsilon())
    {
      const math::quat rot           = math::quat_init_with_array(trans.rotation);
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
    if(math::ray_test_triangles(ray, actor->nav_mesh, actor->nav_mesh_count, &distance))
    {
      math::vec3 scale = math::vec3_scale(math::ray_direction(ray), distance);
      math::vec3 hit = math::vec3_add(ray.start, scale);
    
//      const math::vec3 height = math::vec3_init(0, actor->height, 0);
//      const math::vec3 pos    = math::vec3_add(hit, height);
      const math::vec3 pos = hit;
      
      memcpy(trans.position, pos.data, sizeof(trans.position));
      Nil::Data::set(node, trans);
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
      
      const math::vec3 curr_pos = math::vec3_init_with_array(trans.position);
      
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
        
        memcpy(trans.position, pos.data, sizeof(trans.position));
        Nil::Data::set(node, trans);
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
        
        const math::vec3 curr_pos = math::vec3_init_with_array(trans.position);
        
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
          
          memcpy(trans.position, pos.data, sizeof(trans.position));
          Nil::Data::set(node, trans);
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
      math::vec3_init_with_array(&actor->nav_mesh[index + 0]),
      math::vec3_init_with_array(&actor->nav_mesh[index + 3]),
      math::vec3_init(1,0,0)
    );

    Game_data::debug_line
    (
      math::vec3_init_with_array(&actor->nav_mesh[index + 3]),
      math::vec3_init_with_array(&actor->nav_mesh[index + 6]),
      math::vec3_init(1,0,0)
    );

    Game_data::debug_line
    (
      math::vec3_init_with_array(&actor->nav_mesh[index + 6]),
      math::vec3_init_with_array(&actor->nav_mesh[index + 0]),
      math::vec3_init(1,0,0)
    );
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
    constexpr char static_items[] = "/Users/PhilCK/Desktop/rep_of_a/assets/they_never_pay/mesh/nav_mesh.obj";
    lib::model model = lib::model_import::load_obj_from_file(static_items);
    
    actor->nav_mesh = model.verts[0];
    actor->nav_mesh_count = model.triangle_count[0];
  }
  
  // Main Entity
  {
    Nil::Data::Transform trans{};
    
    float pos[] = {1.f, 0.f, 0.f};
    memcpy(trans.position, pos, sizeof(trans.position));
    
    float scale[] = {1.f, 1.f, 1.f};
    memcpy(trans.scale, scale, sizeof(trans.scale));
    
    float rot[] = {0.f, 0.f, 0.f, 1.f};
    memcpy(trans.rotation, rot, sizeof(trans.rotation));
    
    Nil::Data::set(actor->entity, trans);
  }
  
  // Head
  {
    Nil::Node head;
    head.set_name("Head");
    head.set_parent(actor->entity);
    
    actor->head = head;
    
    // Head Trans
    {
      Nil::Data::Transform trans{};
      
      float pos[] = {0.f, math::g_ratio(), 0.f};
      memcpy(trans.position, pos, sizeof(trans.position));
      
      float scale[] = {1.f, 1.f, 1.f};
      memcpy(trans.scale, scale, sizeof(trans.scale));
      
      float rot[] = {0.f, 0.f, 0.f, 1.f};
      memcpy(trans.rotation, rot, sizeof(trans.rotation));
      
      Nil::Data::set(head, trans);
    }
    
    // Camera
    {
      Nil::Node camera;
      camera.set_name("Camera");
      camera.set_parent(head);
    
      // Transform
      {
        Nil::Data::Transform trans{};
        
        float rot[] = {0.f, 0.f, 0.f, 1.f};
        memcpy(trans.rotation, rot, sizeof(trans.rotation));
        
        Nil::Data::set(camera, trans);
      }
      
      // Camera
      {
        Nil::Data::Camera cam_data{};
        
        cam_data.width              = 1.f;
        cam_data.height             = 1.f;
        cam_data.fov                = math::tau() * 0.12f;
        cam_data.near_plane         = 0.1f;
        cam_data.far_plane          = 100.f;
        cam_data.clear_color_buffer = true;
        cam_data.clear_depth_buffer = true;
        
        Nil::Data::set(camera, cam_data);
      }
    } // cam
  } // head
  
  // Body
  {
    Nil::Node body;
    body.set_name("Body");
    body.set_parent(actor->entity);
    
    // Body Transform
    {
      Nil::Data::Transform trans{};
      
      float pos[] = {0.f, 0.f, 0.f};
      memcpy(trans.position, pos, sizeof(trans.position));
      
      float scale[] = {1.f, math::g_ratio(), 1.f};
      memcpy(trans.scale, scale, sizeof(trans.scale));
      
      float rot[] = {0.f, 0.f, 0.f, 1.f};
      memcpy(trans.rotation, rot, sizeof(trans.rotation));
      
      Nil::Data::set(body, trans);
    }
  }
  
  // Callbacks
  {
    Nil::Data::Logic logic{};
    
    logic.logic_id = 1;
    logic.user_data = (uintptr_t)actor;
    
    logic.think_01 = think;
    
    Nil::Data::set(actor->entity, logic);
  }
  
  // Do one think to get actor into the right place
  {
    think(actor->entity, (uintptr_t)actor);
  }
}


} // ns
