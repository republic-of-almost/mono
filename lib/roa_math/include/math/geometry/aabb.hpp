#ifndef AABB_INCLUDED_0C5E26F6_BFB1_422A_948B_0E4861927EEE
#define AABB_INCLUDED_0C5E26F6_BFB1_422A_948B_0E4861927EEE


#include "../detail/detail.hpp"
#include "geometry_types.hpp"
#include "../vec/vec3.hpp"
#include "../general/general.hpp"
#include <stddef.h>
#include <assert.h>


// --------------------------------------------------------------- [ Config ] --


#define MATH_AABB_INLINE inline


_MATH_NS_OPEN


// ----------------------------------------------------------- [ Initialize ] --


MATH_AABB_INLINE aabb         aabb_init();
MATH_AABB_INLINE aabb         aabb_init(const vec3 min, const vec3 max);
MATH_AABB_INLINE aabb         aabb_init(const float min[3], const float max[3]);
MATH_AABB_INLINE aabb         aabb_init(const vec3 center, const float scale);
MATH_AABB_INLINE aabb         aabb_init(const float vertex[], const size_t number_of_floats);


// ----------------------------------------------------------------- [ Data ] --


MATH_AABB_INLINE vec3         aabb_get_extents(const aabb &a);
MATH_AABB_INLINE vec3         aabb_get_half_extents(const aabb &a);
MATH_AABB_INLINE vec3         aabb_get_min(const aabb &a);
MATH_AABB_INLINE vec3         aabb_get_max(const aabb &a);


// ----------------------------------------------------------- [ Operations ] --


MATH_AABB_INLINE aabb         aabb_combine(const aabb &a, const aabb &b);
MATH_AABB_INLINE void         aabb_scale(aabb &aabb_to_scale, const vec3 scale);
MATH_AABB_INLINE void         aabb_scale(aabb &aabb_to_scale, const float scale);

MATH_AABB_INLINE vec3         aabb_get_origin(const aabb &a);
MATH_AABB_INLINE void         aabb_set_origin(aabb &aabb_to_move, const vec3 new_origin);


// ----------------------------------------------------------------- [ Test ] --


MATH_AABB_INLINE bool         aabb_intersection_test(const aabb &a, const aabb &b);


// ------------------------------------------------------------ [ Init Impl ] --


aabb
aabb_init()
{
  return aabb_init(MATH_NS_NAME::vec3_zero(), MATH_NS_NAME::vec3_zero());
}


aabb
aabb_init(const vec3 min, const vec3 max)
{
  aabb return_aabb;

  return_aabb.max = max;
  return_aabb.min = min;

  return return_aabb;
}


aabb
aabb_init(const float min[3], const float max[3])
{
  return aabb_init(math::vec3_init(min), math::vec3_init(max));
}


aabb
aabb_init(const vec3 center, const float scale)
{
  const float half_scale = MATH_NS_NAME::abs(scale) * 0.5f;

  aabb return_aabb;

  return_aabb.max = vec3_add(vec3_scale(vec3_one(), +half_scale), center);
  return_aabb.min = vec3_add(vec3_scale(vec3_one(), -half_scale), center);

  return return_aabb;
}


aabb
aabb_init(
  const float vertex[],
  const size_t number_of_floats)
{
  aabb out_aabb = aabb_init();

  if(number_of_floats == 0)
  {
    return out_aabb;
  }

  // Check is valid.
  assert((number_of_floats % 3) == 0);
  if((number_of_floats % 3) != 0)
  {
    return out_aabb;
  }

  // Calculate min, max
  float max_x(float_min());
  float max_y(float_min());
  float max_z(float_min());

  float min_x(float_max());
  float min_y(float_max());
  float min_z(float_max());

  const size_t count = number_of_floats / 3;

  for(size_t i = 0; i < count; ++i)
  {
    size_t index = i * 3;

    const float x_val = vertex[index + 0];

    max_x = max(x_val, max_x);
    min_x = min(x_val, min_x);

    const float y_val = vertex[index + 1];
    max_y = max(y_val, max_y);
    min_y = min(y_val, min_y);

    const float z_val = vertex[index + 2];
    max_z = max(z_val, max_z);
    min_z = min(z_val, min_z);
  }

  out_aabb.max = vec3_init(max_x, max_y, max_z);
  out_aabb.min = vec3_init(min_x, min_y, min_z);

  return out_aabb;
}


// ------------------------------------------------------------ [ Data Impl ] --


vec3
aabb_get_extents(const aabb &a)
{
  const vec3 extent = vec3_subtract(a.max, a.min);
  const float x = MATH_NS_NAME::abs(vec3_get_x(extent));
  const float y = MATH_NS_NAME::abs(vec3_get_y(extent));
  const float z = MATH_NS_NAME::abs(vec3_get_z(extent));

  return vec3_init(x, y, z);
}


vec3
aabb_get_half_extents(const aabb &a)
{
  return vec3_scale(aabb_get_extents(a), 0.5f);
}


vec3
aabb_get_min(const aabb &a)
{
  return a.min;
}


vec3
aabb_get_max(const aabb &a)
{
  return a.max;
}


// ------------------------------------------------------ [ Operations Impl ] --


aabb
aabb_combine(const aabb &a, const aabb &b)
{
  return aabb_init(
    MATH_NS_NAME::vec3_init(
      MATH_NS_NAME::min(MATH_NS_NAME::get_x(a.min), MATH_NS_NAME::get_x(b.min)),
      MATH_NS_NAME::min(MATH_NS_NAME::get_y(a.min), MATH_NS_NAME::get_y(b.min)),
      MATH_NS_NAME::min(MATH_NS_NAME::get_z(a.min), MATH_NS_NAME::get_z(b.min))
    ),
    MATH_NS_NAME::vec3_init(
      MATH_NS_NAME::max(MATH_NS_NAME::get_x(a.max), MATH_NS_NAME::get_x(b.max)),
      MATH_NS_NAME::max(MATH_NS_NAME::get_y(a.max), MATH_NS_NAME::get_y(b.max)),
      MATH_NS_NAME::max(MATH_NS_NAME::get_z(a.max), MATH_NS_NAME::get_z(b.max))
    )
  );
}

vec3
aabb_get_origin(const aabb &a)
{
  const vec3 half_extent = aabb_get_half_extents(a);

  return vec3_add(a.min, half_extent);
}


void
aabb_scale(aabb &aabb_to_scale, const vec3 scale)
{
  aabb_to_scale.max = MATH_NS_NAME::vec3_multiply(aabb_to_scale.max, scale);
  aabb_to_scale.min = MATH_NS_NAME::vec3_multiply(aabb_to_scale.min, scale);
}


void
aabb_scale(aabb &aabb_to_scale, const float scale)
{
  return aabb_scale(aabb_to_scale, vec3_init(scale));
}


void
aabb_set_origin(aabb &aabb_to_move, const vec3 new_position)
{
  const vec3 diff = vec3_subtract(new_position, aabb_get_origin(aabb_to_move));
  aabb_to_move.min = vec3_add(aabb_to_move.min, diff);
  aabb_to_move.max = vec3_add(aabb_to_move.max, diff);
}


// ------------------------------------------------------------ [ Test Impl ] --


namespace detail
{
  // Simple Single Axis Therom test.
  // We assume we are dealing with just AABB boxes.
  inline bool
  sat_test(const float origin_a,
           const float origin_b,
           const float combined_length)
  {
    return MATH_NS_NAME::abs(origin_b - origin_a) < combined_length;
  }
} // ns


bool
aabb_intersection_test(const aabb &a,
                       const aabb &b)
{
  const vec3 origin_a = aabb_get_origin(a);
  const vec3 origin_b = aabb_get_origin(b);

  const vec3 half_ext_a = aabb_get_half_extents(a);
  const vec3 half_ext_b = aabb_get_half_extents(b);
  const vec3 combined_half_extent = vec3_add(half_ext_a, half_ext_b);

  return (
    detail::sat_test(
      MATH_NS_NAME::vec3_get_x(origin_a),
      MATH_NS_NAME::vec3_get_x(origin_b),
      MATH_NS_NAME::vec3_get_x(combined_half_extent)
    ) &&
    detail::sat_test(
      MATH_NS_NAME::vec3_get_y(origin_a),
      MATH_NS_NAME::vec3_get_y(origin_b),
      MATH_NS_NAME::vec3_get_y(combined_half_extent)
    ) &&
    detail::sat_test(
      MATH_NS_NAME::vec3_get_z(origin_a),
      MATH_NS_NAME::vec3_get_z(origin_b),
      MATH_NS_NAME::vec3_get_z(combined_half_extent)
    )
  );
}


_MATH_NS_CLOSE


#endif // inc guard
