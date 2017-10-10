#ifndef CAMERA_INCLUDED_6E78C415_FE13_483C_97A1_8767137A48AC
#define CAMERA_INCLUDED_6E78C415_FE13_483C_97A1_8767137A48AC


#include "object.hpp"


namespace AS_utils {


using Camera = Object;


/* utils */

void      camera_ctor(void *mem);
void      camera_dtor(void *mem);

Camera    *camera_new();


/* methods */

void      camera_set_fov(const float fov, Camera *cam);
float     camera_get_fov(const Camera *cam);

void      camera_set_width(const float width, Camera *cam);
float     camera_get_width(const Camera *cam);

void      camera_set_height(const float height, Camera *cam);
float     camera_get_height(const Camera *cam);

void      camera_set_nearp(const float near, Camera *cam);
float     camera_get_nearp(const Camera *cam);

void      camera_set_farp(const float far, Camera *cam);
float     camera_get_farp(const Camera *cam);

void      camera_set_priority(const int priority, Camera *cam);
int       camera_get_priority(const Camera *cam);

void      camera_set_depth_clear(const bool clear, Camera *cam);
bool      camera_get_depth_clear(const Camera *cam);

void      camera_set_color_clear(const bool clear, Camera *cam);
bool      camera_get_color_clear(const Camera *cam);


} // ns


#endif
