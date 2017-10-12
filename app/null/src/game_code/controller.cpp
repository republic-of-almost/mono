#include <game_code/controller.hpp>
#include <math/general/general.hpp>


void
controller_update(Controller *controllers, const size_t count)
{
  ROA_ASSERT(count == 1); // ROA needs gamepad support before we can handle more than one.
  
  // TODO: We can get rid of all branches here.
  // by making key_state 1 or 0, then multiplying that by move factor.
  
  // Keyboard
  {
    const bool fwd_down   = ROA::Keyboard::key_state(ROA::KeyCode::W) & ROA::KeyState::DOWN;
    const bool back_down  = ROA::Keyboard::key_state(ROA::KeyCode::S) & ROA::KeyState::DOWN;
    const bool left_down  = ROA::Keyboard::key_state(ROA::KeyCode::A) & ROA::KeyState::DOWN;
    const bool right_down = ROA::Keyboard::key_state(ROA::KeyCode::D) & ROA::KeyState::DOWN;
    
    float fwd = 0.f;
    fwd += fwd_down ? +1.f : 0.f;
    fwd += back_down ? -1.f : 0.f;
    
    float left = 0.f;
    left += left_down ? -1.f : 0.f;
    left += right_down ? +1.f : 0.f;
    
    if(math::is_near(math::abs(fwd), 1.f) && math::is_near(math::abs(left), 1.f))
    {
      left *= math::root_two() * 0.5f;
      fwd *= math::root_two() * 0.5f;
    }
    
    controllers[0].fwd = fwd;
    controllers[0].left = left;
  }
  
  // Mouse Delta
  {
    controllers[0].lookat_axis[0] += (ROA::Mouse::get_delta().x / 100.f);
    controllers[0].lookat_axis[1] += (ROA::Mouse::get_delta().y / 100.f);
  }
  
  // Mouse Click
  {
    const bool click = ROA::Keyboard::key_state(ROA::KeyCode::SPACE) & ROA::KeyState::DOWN;
    
    if(click)
    {
//      controllers[0].ray = Ray();
    }
  }
}

