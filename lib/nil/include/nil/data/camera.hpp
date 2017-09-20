#ifndef CAMERA_INCLUDED_D283F4AA_69AE_4E3F_9582_F363039E606B
#define CAMERA_INCLUDED_D283F4AA_69AE_4E3F_9582_F363039E606B


#include <nil/fwd.hpp>


namespace Nil {
namespace Data {


// ----------------------------------------------------------------- [ Data ] --


struct Camera
{
  enum { PERSPECTIVE, ORTHOGRAPHIC } type;
  
  int32_t priority;
  float width;
  float height;
  float fov;
  float near_plane;
  float far_plane;
  
  uint32_t clear_color;
  
  bool clear_color_buffer;
  bool clear_depth_buffer;
  
  // -- Output -- //
  
  float position[3];
  float view_mat[16];
  
};


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Camera **cameras);


void
get(const Node &node, Camera &out);


void
set(Node &node, const Camera &in);


void
remove_camera(Node &node);


// ----------------------------------------------------------------- [ Info ] --


bool
has_camera(const Node &node);


bool
has(const Node &node, const Camera &data);


uint64_t
get_type_id(const Camera &in);


const char*
get_type_name(const Camera &in);


size_t
camera_count();


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Camera **out_data, Node **out_node);


} // ns
} // ns


#endif // inc guard
