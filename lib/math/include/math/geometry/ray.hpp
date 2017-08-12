#ifndef RAY_INCLUDED_9580B5C2_5FD1_4185_9537_C6C4F7EC3A4E
#define RAY_INCLUDED_9580B5C2_5FD1_4185_9537_C6C4F7EC3A4E


#include "../detail/detail.hpp"
#include "geometry_types.hpp"
#include "../vec/vec.hpp"
#include "../mat/mat.hpp"


_MATH_NS_OPEN


// ----------------------------------------------------------- [ Interface ] --


inline ray        ray_init(const vec3 start, const vec3 end);
inline ray        ray_inverse(const ray &inverse);
inline float      ray_length(const ray &ray);
inline vec3       ray_direction(const ray &ray);

inline float      ray_test_aabb(const ray &ray, const aabb &target);
inline bool       ray_test_plane(const ray &ray, const plane &target, float *out_distance = nullptr);
inline bool       ray_test_triangles(const ray &in_ray, const float tris[], const size_t tri_count, float *out_distance = nullptr);
inline bool       ray_test_closest_edge(const float tris[], const size_t tri_count, const vec3 point, vec3 &seg_a, vec3 &seg_b);

inline ray        ray_from_perpective_viewport(
  const float vp_width,
  const float vp_height,
  const float vp_coord_x,
  const float vp_coord_y,
  const mat4 &proj_mat,
  const mat4 &view_mat,
  const vec3 ray_start,
  const float distance);



// ---------------------------------------------------------------- [ Impl ] --


ray
ray_init(const vec3 start, const vec3 end)
{
  return ray{start, end};
}


ray
ray_inverse(const ray &ray)
{
  return ray_init(ray.end, ray.start);
}


float
ray_length(const ray &ray)
{
  return vec3_length(vec3_subtract(ray.end, ray.start));
}


vec3
ray_direction(const ray &ray)
{
  return vec3_normalize(vec3_subtract(ray.end, ray.start));
}


float
ray_test_aabb(const ray &in_ray, const aabb &box)
{
  const vec3 ray_dir = ray_direction(in_ray);
  constexpr float no_hit = 0.f;

  float t[10];
  
  t[1] = (get_x(box.min) - get_x(in_ray.start)) / get_x(ray_dir);
  t[2] = (get_x(box.max) - get_x(in_ray.start)) / get_x(ray_dir);
  t[3] = (get_y(box.min) - get_y(in_ray.start)) / get_y(ray_dir);
  t[4] = (get_y(box.max) - get_y(in_ray.start)) / get_y(ray_dir);
  t[5] = (get_z(box.min) - get_z(in_ray.start)) / get_z(ray_dir);
  t[6] = (get_z(box.max) - get_z(in_ray.start)) / get_z(ray_dir);
  
  t[7] = MATH_NS_NAME::max(
           MATH_NS_NAME::max(
             MATH_NS_NAME::min(t[1], t[2]),
             MATH_NS_NAME::min(t[3], t[4])
           ),
           MATH_NS_NAME::min(t[5], t[6])
         );
  
  
  t[8] = MATH_NS_NAME::min(
           MATH_NS_NAME::min(
             MATH_NS_NAME::max(t[1], t[2]),
             MATH_NS_NAME::max(t[3], t[4])
           ),
           MATH_NS_NAME::max(t[5], t[6])
         );
  
  t[9] = (t[8] < 0 || t[7] > t[8]) ? no_hit : t[7];
  
  return t[9];
}


bool
ray_test_plane(const ray &in_ray, const plane &target, float *out_distance)
{
  const vec3 ray_dir = ray_direction(in_ray);

  const float dot = vec3_dot(target.normal, ray_dir);
  
  if (MATH_NS_NAME::abs(dot) > MATH_NS_NAME::epsilon())
  {
    const vec3 distance = vec3_subtract(target.position, in_ray.start);
    const float dot_norm = vec3_dot(distance, target.normal) / dot;
    
    if(out_distance)
    {
      *out_distance = dot_norm;
    }
    
    if(dot_norm >= 0)
    {
      return true;
    }
  }
  
  return false;
}


bool
ray_test_triangles(
  const ray &in_ray,
  const float tris[],
  const size_t tri_count,
  float *out_distance)
{
  const vec3 r_dir = MATH_NS_NAME::ray_direction(in_ray);
  
  float distance = 10000000000.0;
  bool found = false;
  
  for(size_t i = 0; i < tri_count; ++i)
  {
    const size_t tri_index = i * 3 * 3;
   
    const vec3 v0 = MATH_NS_NAME::vec3_init(
      &tris[tri_index]
    );
    
    const vec3 v1 = MATH_NS_NAME::vec3_subtract(
      MATH_NS_NAME::vec3_init(&tris[tri_index + 3]), v0
    );
    
    const vec3 v2 = MATH_NS_NAME::vec3_subtract(
      MATH_NS_NAME::vec3_init(&tris[tri_index + 6]), v0
    );

    const vec3 p_vec = MATH_NS_NAME::vec3_cross(r_dir, v2);
    const float dot  = MATH_NS_NAME::vec3_dot(v1, p_vec);

    if(dot < MATH_NS_NAME::epsilon())
    {
      continue;
    }

    const float o_dot = 1 / dot;
    const vec3 t_vec  = MATH_NS_NAME::vec3_subtract(in_ray.start, v0);
    const float u     = MATH_NS_NAME::vec3_dot(t_vec, p_vec) * o_dot;
  
    if(!MATH_NS_NAME::is_between(u, 0.f, 1.f))
    {
      continue;
    }
 
    const vec3 q_vec = MATH_NS_NAME::vec3_cross(t_vec, v1);
    const float v    = MATH_NS_NAME::vec3_dot(r_dir, q_vec) * o_dot;
  
    if (v < 0 || u + v > 1)
    {
      continue;
    }
 
    const float t = MATH_NS_NAME::vec3_dot(v2, q_vec) * o_dot;
  
    if(t > MATH_NS_NAME::epsilon() && t < distance)
    {
      distance = t;
      found = true;
    }
  }
  
  if(found)
  {
    *out_distance = distance;
  }
  
  return found;
}


bool
ray_test_closest_edge(const float tris[], const size_t tri_count, const vec3 point, vec3 &seg_a, vec3 &seg_b)
{
  constexpr float max_dist = 10000000000.0;
  
  float curr_closest = max_dist;
  
  vec3 curr_segment[2];
  
  for(size_t i = 0; i < tri_count; ++i)
  {
    const size_t tri_index = i * 3 * 3;
    
    const vec3 verts[] {
      vec3_init(&tris[tri_index + 0]),
      vec3_init(&tris[tri_index + 3]),
      vec3_init(&tris[tri_index + 6]),
    };
    
    // Segments
    for(uint32_t j = 0; j < 3; ++j)
    {
      const size_t va = j;
      const size_t vb = (j + 1) % 3;
    
      vec3 v = vec3_subtract(verts[vb], verts[va]);
      vec3 w = vec3_subtract(point, verts[va]);
      
      float c1 = vec3_dot(w, v);
      
      if(c1 <= 0)
      {
        const float dist = vec3_length(vec3_subtract(point, verts[va]));
        
        if(dist < curr_closest)
        {
          curr_closest = dist;
          curr_segment[0] = verts[va];
          curr_segment[1] = verts[vb];
        }
        continue;
      }
      
      float c2 = vec3_dot(v, v);
      if(c2 <= c1)
      {
        const float dist = vec3_length(vec3_subtract(point, verts[vb]));
        
        if(dist < curr_closest)
        {
          curr_closest = dist;
          curr_segment[0] = verts[va];
          curr_segment[1] = verts[vb];
        }
        continue;
      }
      
      float b = c1 / c2;
      
      vec3 p = vec3_add(verts[va], vec3_scale(v, b));
      
      const float dist = vec3_length(vec3_subtract(point, p));
      
      if(dist < curr_closest)
      {
        curr_closest = dist;
        curr_segment[0] = verts[va];
        curr_segment[1] = verts[vb];
      }
    }
  }
  
  seg_a = curr_segment[0];
  seg_b = curr_segment[1];
  
  return curr_closest < max_dist;
}


ray
ray_from_perpective_viewport(
  const float vp_width,
  const float vp_height,
  const float vp_coord_x,
  const float vp_coord_y,
  const mat4 &proj_mat,
  const mat4 &view_mat,
  const vec3 ray_start,
  const float distance)
{
  // Some Mouse things
//  const Axis viewport_size {
//    math::to_float(camera.get_width()),
//    math::to_float(camera.get_height())
//  };
  
  const math::vec2 viewport_size = math::vec2_init(vp_width, vp_height);
  
//  const Axis clamped_viewport {
//    math::clamp(viewport_coords.x, 0, viewport_size.x),
//    math::clamp(viewport_coords.y, 0, viewport_size.y)
//  };
  
  const math::vec2 clamped_viewport = math::vec2_init(
    math::clamp(vp_coord_x, 0.f, math::get_x(viewport_size)),
    math::clamp(vp_coord_y, 0.f, math::get_y(viewport_size))
  );

//  if(camera.get_type() == Core::Camera_type::perspective)
//  {
  const math::vec2 coords = math::vec2_init(
    (((2.f * math::get_x(clamped_viewport)) / math::get_x(viewport_size)) - 1.f),
    1.f - (2.f * math::get_y(clamped_viewport)) / math::get_y(viewport_size)
  );

  // --
  
  const math::vec3 ray_nds  = math::vec3_init(math::get_x(coords), math::get_y(coords), 1.f);
  const math::vec4 ray_clip = math::vec4_init(math::get_x(ray_nds),
                                              math::get_y(ray_nds),
                                              1.f,
                                              1.f);
  
  const math::mat4 proj_inv_mat = math::mat4_get_inverse(proj_mat);
  
  const math::vec4 ray_eye_get  = math::mat4_multiply(ray_clip, proj_inv_mat);
  const math::vec4 ray_eye      = math::vec4_init(math::get_x(ray_eye_get),
                                                  math::get_y(ray_eye_get),
                                                  -1.f,
                                                  0.f);
  
  // --
  
//  const math::mat4 view_mat     = Core::Camera_utils::get_view_matrix(camera);
  const math::mat4 view_inv_mat = math::mat4_get_inverse(view_mat);
  
  const math::vec4 ray_wor_all = math::mat4_multiply(ray_eye, view_inv_mat);
  const math::vec3 ray_wor     = math::vec3_normalize(math::vec3_init(math::get_x(ray_wor_all),
                                                                      math::get_y(ray_wor_all),
                                                                      math::get_z(ray_wor_all)));

  // --

//  const math::vec3 ray_start = camera.get_attached_entity().get_transform().get_position();
  const math::vec3 ray_dir   = ray_wor;
  
//  return Core::Ray(ray_start, ray_dir);

  math::vec3 ray_end = math::vec3_add(ray_start, math::vec3_scale(ray_dir, distance));
  return math::ray_init(ray_start, ray_end);
  
//  }
//  else if(camera.get_type() == Core::Camera_type::orthographic)
//  {
//    const Core::Axis coords {
//      ((viewport_size.x * 0.5f) - clamped_viewport.x) * -1.f,
//      (viewport_size.y * 0.5f) - clamped_viewport.y
//    };
//    
//    const Core::Transform cam_tran = camera.get_attached_entity().get_transform();
//    
//    const math::vec3 left = math::vec3_scale(cam_tran.get_left(), coords.x);
//    const math::vec3 up   = math::vec3_scale(cam_tran.get_up(), coords.y);
//    
//    const math::vec3 cam_pos = cam_tran.get_position();
//    
//    const math::vec3 ray_start = math::vec3_add(math::vec3_add(cam_pos, left), up);
//    const math::vec3 ray_dir   = cam_tran.get_forward();
//
//    return Core::Ray(ray_start, ray_dir);
//  }
//  else
//  {
//    assert(false);
//    LOG_FATAL("This is unreachable code");
//    return Core::Ray(math::vec3_zero(), math::vec3_zero_zero_one());
//  }
  
  // UNREACHABLE //
}


_MATH_NS_CLOSE


#endif // inc guard
