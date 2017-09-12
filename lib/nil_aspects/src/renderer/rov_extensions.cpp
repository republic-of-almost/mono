#include "rov_extensions.hpp"
#include <nil/data/bounding_box.hpp>
#include <nil/data/camera.hpp>
#include <rov/rov.hpp>
#include <math/math.hpp>


namespace Nil_ext {


void
rov_render_camera_cross(Nil::Data::Camera cam, uint32_t viewport[2])
{
  /*
    This needs so much work.
  */
  
  // Generate ray.
  math::mat4 proj_mat = math::mat4_perspective(
    cam.width * viewport[0],
    cam.height * viewport[1],
    cam.near_plane,
    cam.far_plane,
    cam.fov
  );

  math::mat4 view_mat = math::mat4_init(cam.view_mat);

  math::ray r = math::ray_from_perpective_viewport(
    cam.width * viewport[0],
    cam.height * viewport[1],
    viewport[0] / 2,
    viewport[1] / 2,
    proj_mat,
    view_mat,
    math::vec3_init(cam.position),
    100000
  );
  
  math::vec3 pos = math::vec3_init(cam.position);
  math::vec3 dir = math::ray_direction(r);
  math::plane p = math::plane_init(math::vec3_add(pos, math::vec3_scale(dir, cam.near_plane)), math::vec3_scale(dir, -1.f));
  
  float dist = 0.f;
  ray_test_plane(r, p, &dist);
  
  if(dist)
  {
    pos = math::vec3_add(pos, math::vec3_scale(dir, dist * 2));
    
    rov_setColor(1,1,1,1);
    
    const float a[3] { math::get_x(pos), math::get_y(pos), math::get_z(pos) };
    const float b[3] { math::get_x(pos), math::get_y(pos) + 0.001f, math::get_z(pos) };
    
    rov_submitLine(a, b);
  }
}


void
rov_render_bounding_box(
  Nil::Data::Bounding_box bbs[],
  size_t count)
{
  for(size_t i = 0; i < count; ++i)
  {
    const float *min = bbs[i].min;
    const float *max = bbs[i].max;

    // Y lines
    {
      const float a[3] { min[0], min[1], min[2] };
      const float b[3] { min[0], max[1], min[2] };
      rov_submitLine(a, b);

      const float c[3] { max[0], min[1], min[2] };
      const float d[3] { max[0], max[1], min[2] };
      rov_submitLine(c, d);

      const float e[3] { min[0], min[1], max[2] };
      const float f[3] { min[0], max[1], max[2] };
      rov_submitLine(e, f);

      const float g[3] { max[0], min[1], max[2] };
      const float h[3] { max[0], max[1], max[2] };
      rov_submitLine(g, h);
    }

    // X Lines
    {
      const float a[3] { min[0], min[1], min[2] };
      const float b[3] { max[0], min[1], min[2] };
      rov_submitLine(a, b);

      const float c[3] { min[0], max[1], min[2] };
      const float d[3] { max[0], max[1], min[2] };
      rov_submitLine(c, d);

      const float e[3] { min[0], min[1], max[2] };
      const float f[3] { max[0], min[1], max[2] };
      rov_submitLine(e, f);

      const float g[3] { min[0], max[1], max[2] };
      const float h[3] { max[0], max[1], max[2] };
      rov_submitLine(g, h);
    }

    // Z lines
    {
      const float a[3] { min[0], min[1], min[2] };
      const float b[3] { min[0], min[1], max[2] };
      rov_submitLine(a, b);

      const float c[3] { max[0], min[1], min[2] };
      const float d[3] { max[0], min[1], max[2] };
      rov_submitLine(c, d);

      const float e[3] { min[0], max[1], min[2] };
      const float f[3] { min[0], max[1], max[2] };
      rov_submitLine(e, f);

      const float g[3] { max[0], max[1], min[2] };
      const float h[3] { max[0], max[1], max[2] };
      rov_submitLine(g, h);
    }
  }
}


} // ns
