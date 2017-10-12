#ifndef FREE_CAM_INCLUDED_8A85BDC1_E642_4629_9EE3_2C9FBD1BC7AF
#define FREE_CAM_INCLUDED_8A85BDC1_E642_4629_9EE3_2C9FBD1BC7AF


#include <game_code/fundamental.hpp>
#include <roa/fundamental.hpp>


#define FREE_CAM_SPEED_MULTIPLIER 0.3


struct Free_cam {

  float pitch;
  float yaw;
  float fwd;
  float left;
};


void
free_cam_update(Free_cam *free_cam_arr, ROA::Object *cam_arr, size_t count);


void
free_cam_apply_controller(Free_cam *free_cam_arr, Controller *controllers, size_t count);


#endif // inc guard
