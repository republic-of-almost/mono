#include "camera.hpp"
#include <new>


namespace AS_utils {


using Camera = Object;


/* utils */

void
camera_ctor(void *mem)
{
  new(mem) Camera();
}


void
camera_dtor(void *mem)
{
  ((Camera*)mem)->~Camera();
}


Camera*
camera_new()
{
  return new(Camera);
}


/* methods */

void
camera_set_fov(const float fov, Camera *cam)
{
  
}


float
camera_get_fov(const Camera *cam)
{
  return 0.f;
}


void
camera_set_width(const float width, Camera *cam)
{
}


float
camera_get_width(const Camera *cam)
{
  return 0.f;
}


void
camera_set_height(const float height, Camera *cam)
{
}


float
camera_get_height(const Camera *cam)
{
  return 0.f;
}


void
camera_set_nearp(const float near, Camera *cam)
{
}


float
camera_get_nearp(const Camera *cam)
{
  return 0.f;
}


void
camera_set_farp(const float far, Camera *cam)
{
}


float
camera_get_farp(const Camera *cam)
{
  return 0.f;
}


void
camera_set_priority(const int priority, Camera *cam)
{
}


int
camera_get_priority(const Camera *cam)
{
  return 0;
}


void
camera_set_depth_clear(const bool clear, Camera *cam)
{
}


bool
camera_get_depth_clear(const Camera *cam)
{
  return false;
}


void
camera_set_color_clear(const bool clear, Camera *cam)
{
}


bool
camera_get_color_clear(const Camera *cam)
{
  return false;
}


} // ns
