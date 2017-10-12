#include <game_code/freecam.hpp>
#include <game_code/controller.hpp>
#include <roa/roa.hpp>
#include <stdio.h>


void
free_cam_apply_controller(Free_cam *free_cam_arr, Controller *controllers, size_t count)
{
  for(size_t i = 0; i < count; ++i)
  {
    free_cam_arr[i].pitch = controllers[i].lookat_axis[1];
    free_cam_arr[i].yaw = controllers[i].lookat_axis[0];
    free_cam_arr[i].fwd = controllers[i].fwd * FREE_CAM_SPEED_MULTIPLIER;
    free_cam_arr[i].left = controllers[i].left * FREE_CAM_SPEED_MULTIPLIER;
  }
}


void
free_cam_update(Free_cam *free_cam_arr, ROA::Object *cam_arr, size_t count)
{
  for(size_t i = 0; i < count; ++i)
  {
    Free_cam *cam = &free_cam_arr[i];
    ROA::Transform trans = cam_arr[i].get_data<ROA::Transform>();
    
    // Position
    {
      const ROA::Vector3 fwd  = trans.get_forward().scale(cam->fwd);
      const ROA::Vector3 left = trans.get_left().scale(cam->left);
      const ROA::Vector3 pos  = trans.get_position().add(fwd).add(left);
      
      trans.set_position(pos);
    }
  
    // Rotation
    {
      const ROA::Quaternion pitch(ROA::Vector3(1,0,0), cam->pitch);
      const ROA::Quaternion yaw(ROA::Vector3(0,1,0), cam->yaw);
      const ROA::Quaternion rotation(yaw.multiply(pitch));
      
      trans.set_rotation(rotation);
    }
  }
}
