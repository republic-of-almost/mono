#ifndef CAMERA_INCLUDED_D283F4AA_69AE_4E3F_9582_F363039E606B
#define CAMERA_INCLUDED_D283F4AA_69AE_4E3F_9582_F363039E606B


#include <nil/fwd.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Camera
{
  enum { PERSPECTIVE, ORTHOGRAPHIC } type;
  
  uint32_t priority;
  float width;
  float height;
  float fov;
  float near_plane;
  float far_plane;
  
  bool clear_color_buffer;
  bool clear_depth_buffer;
  
  uintptr_t user_data;
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Camera &out);


void
set(Node &node, const Camera &in);


void
remove_camera(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_camera(const Node &node);


uint64_t
get_type_id(const Camera &in);


size_t
camera_count();


// ---------------------------------------------------------- [ Data Events ] --


void
events(const uint32_t data, size_t *count, Camera **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
