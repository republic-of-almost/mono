#include <roa_math/math.h>
#include <string.h>
#include <assert.h>
#include <math.h>


/* ---------------------------------------------------------- [ helpers ] -- */


#define MATH_ARR_COUNT(arr) sizeof(arr[0]) / sizeof(arr)


/* ---------------------------------------------------------- [ general ] -- */


float
roa_float_tan(float a)
{
  return tanf(a);
}


float
roa_float_sin(float a)
{
  return sinf(a);
}


float
roa_float_cos(float a)
{
  return cosf(a);
}


float
roa_float_sqrt(float a)
{
  return sqrtf(a);
}


float
roa_float_abs(float a)
{
  return fabs(a);
}


int
roa_float_is_near(float a, float b, float err)
{
  return (roa_float_abs(b) - roa_float_abs(a)) < err ? 1 : 0;
}


/* ----------------------------------------------------------- [ float2 ] -- */
/* ----------------------------------------------------------- [ float3 ] -- */


roa_float3
roa_float3_zero()
{
  roa_float3 ret{ 0.f, 0.f, 0.f };
  return ret;
}


roa_float3
roa_float3_zero_zero_one()
{
  roa_float3 ret{ 0.f, 0.f, 1.f };
  return ret;
}


roa_float3
roa_float3_one()
{
  roa_float3 ret{ 1.f, 1.f, 1.f };
  return ret;
}

roa_float3
roa_float3_fill_with_value(float v)
{
  roa_float3 ret{ v, v, v };
  return ret;
}


roa_float3
roa_float3_set_with_values(float x, float y, float z)
{
  roa_float3 ret{ x, y, z };
  return ret;
}


float
roa_float3_get_x(roa_float3 a)
{
  return a.x;
}


float
roa_float3_get_y(roa_float3 a)
{
  return a.y;
}


float
roa_float3_get_z(roa_float3 a)
{
  return a.z;
}


roa_float3
roa_float3_add(roa_float3 a, roa_float3 b)
{
  roa_float3 ret;
  
  ret.x = a.x + b.x;
  ret.y = a.y + b.y;
  ret.z = a.z + b.z;

  return ret;
}


roa_float3
roa_float3_subtract(roa_float3 a, roa_float3 b)
{
  roa_float3 ret;

  ret.x = a.x - b.x;
  ret.y = a.y - b.y;
  ret.z = a.z - b.z;

  return ret;
}


roa_float3
roa_float3_multiply(roa_float3 a, roa_float3 b)
{
  roa_float3 ret;

  ret.x = a.x * b.x;
  ret.y = a.y * b.y;
  ret.z = a.z * b.z;

  return ret;
}


float
roa_float3_length(roa_float3 a)
{
  const float len = a.x * a.x + a.y * a.y + a.z * a.z;

  return roa_float_sqrt(len);
}


roa_float3
roa_float3_normalize(roa_float3 a)
{
  const float length = roa_float3_length(a);

  assert(length != 0); // Don't pass zero vectors. (0,0,0);

  return roa_float3_scale(a, (1.f / length));
}


float
roa_float3_dot(roa_float3 a, roa_float3 b)
{
  return (roa_float3_get_x(a) * roa_float3_get_x(b)) +
    (roa_float3_get_y(a) * roa_float3_get_y(b)) +
    (roa_float3_get_z(a) * roa_float3_get_z(b));
}


roa_float3
roa_float3_cross(roa_float3 a, roa_float3 b)
{
  const float x = (roa_float3_get_y(a) * roa_float3_get_z(b)) - (roa_float3_get_z(a) * roa_float3_get_y(b));
  const float y = (roa_float3_get_x(a) * roa_float3_get_z(b)) - (roa_float3_get_z(a) * roa_float3_get_x(b));
  const float z = (roa_float3_get_x(a) * roa_float3_get_y(b)) - (roa_float3_get_y(a) * roa_float3_get_x(b));

  return roa_float3_set_with_values(x, -y, z);
}


roa_float3
roa_float3_scale(roa_float3 a, float scale)
{
  const roa_float3 scale_vec = roa_float3_fill_with_value(scale);
  return roa_float3_multiply(a, scale_vec);
}


int
roa_float3_is_near(roa_float3 a, roa_float3 b, float err)
{
  int x_is_near = roa_float_is_near(a.x, b.x, err);
  int y_is_near = roa_float_is_near(a.y, b.y, err);
  int z_is_near = roa_float_is_near(a.z, b.z, err);

  return (x_is_near + y_is_near + z_is_near) == 3;
}


/* ----------------------------------------------------------- [ float4 ] -- */

/* ------------------------------------------------------- [ quaternion ] -- */


roa_quaternion
roa_quaternion_default()
{
  roa_quaternion quat{
    0.f, 0.f, 0.f, 1.f
  };

  return quat;
}


roa_quaternion
roa_quaternion_set_with_values(float x, float y, float z, float w)
{
  roa_quaternion quat{
    x, y, z, w
  };

  return quat;
}


roa_quaternion
roa_quaternion_multiply(roa_quaternion left, roa_quaternion right)
{
  const float w = (left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z);
  const float x = (left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y);
  const float y = (left.w * right.y) + (left.y * right.w) + (left.z * right.x) - (left.x * right.z);
  const float z = (left.w * right.z) + (left.z * right.w) + (left.x * right.y) - (left.y * right.x);

  return roa_quaternion_set_with_values(x, y, z, w);
}


int
roa_quaternion_is_near(roa_quaternion a, roa_quaternion b, float err)
{
  int x_is_near = roa_float_is_near(a.x, b.x, err);
  int y_is_near = roa_float_is_near(a.y, b.y, err);
  int z_is_near = roa_float_is_near(a.z, b.z, err);
  int w_is_near = roa_float_is_near(a.w, b.w, err);

  return (x_is_near + y_is_near + z_is_near + w_is_near) == 4;
}

/* -------------------------------------------------------- [ transform ] -- */


void
roa_transform_init(roa_transform *out)
{
  assert(out);

  out->position = roa_float3_zero();
  out->scale    = roa_float3_one();
  out->rotation = roa_quaternion_default();
}


void
roa_transform_inherited(
  roa_transform *out,
  const roa_transform *parent,
  const roa_transform *local)
{
  assert(out);
  assert(parent);
  assert(local);

  out->scale = roa_float3_multiply(parent->scale, local->scale);
  out->rotation = roa_quaternion_multiply(parent->rotation, local->rotation);
  out->position = roa_float3_add(
    parent->position,
    roa_float3_multiply(
      local->position,
      parent->scale
    )
  );

}


/* ------------------------------------------------------------- [ mat3 ] -- */
/* ------------------------------------------------------------- [ mat4 ] -- */


void
roa_mat4_zero(roa_mat4 *out)
{
  assert(out);

  memset((void*)out->data, 0, sizeof(out->data));
}


void
roa_mat4_id(roa_mat4 *out)
{
  assert(out);

  roa_mat4_zero(out);

  out->data[0] = 1.f;
  out->data[5] = 1.f;
  out->data[10] = 1.f;
  out->data[15] = 1.f;
}


void
roa_mat4_import(roa_mat4 *out, float *data)
{
  for (int i = 0; i < MATH_ARR_COUNT(out->data); ++i)
  {
    out->data[i] = data[i];
  }
}


void
roa_mat4_fill_with_value(roa_mat4 *out, float val)
{
  assert(out);

  for (int i = 0; i < MATH_ARR_COUNT(out->data); ++i)
  {
    out->data[i] = val;
  }
}


void
roa_mat4_projection(roa_mat4 *out, float fov, float near_plane, float far_plane, float aspect_ratio)
{
  assert(out);

  const float xy_max = near_plane * roa_float_tan(fov);
  const float y_min = -xy_max;
  const float x_min = -xy_max;

  const float width = xy_max - x_min;
  const float height = xy_max - y_min;

  float depth = far_plane - near_plane;
  float q = -(far_plane + near_plane) / depth;
  float qn = -2 * (far_plane * near_plane) / depth;

  float w = 2 * near_plane / width;
  w = w / aspect_ratio;
  float h = 2 * near_plane / height;

  roa_mat4_zero(out);

  out->data[0] = w;
  out->data[5] = h;
  out->data[10] = q;
  out->data[11] = -1;
  out->data[14] = qn;

  //const float one_over_tan_half_fov = 1.f / roa_tan(fov * 0.5f);
  //const float plane_diff = far_plane - near_plane;

  //roa_mat4_zero(out);

  //out->data[0] = one_over_tan_half_fov / aspect_ratio;
  //out->data[5] = one_over_tan_half_fov;
  //out->data[10] = -(far_plane + near_plane) / plane_diff;
  //out->data[11] = -1.f;
  //out->data[14] = -(2.f * far_plane * near_plane) / plane_diff;
}


void
roa_mat4_lookat(roa_mat4 *out, roa_float3 from, roa_float3 to, roa_float3 up)
{
  const roa_float3 z_axis = roa_float3_normalize(roa_float3_subtract(from, to));
  const roa_float3 x_axis = roa_float3_normalize(roa_float3_cross(up, z_axis));
  const roa_float3 y_axis = roa_float3_cross(z_axis, x_axis);

  out->data[0] = +roa_float3_get_x(x_axis);
  out->data[1] = +roa_float3_get_x(y_axis);
  out->data[2] = +roa_float3_get_x(z_axis);
  out->data[3] = 0.f;

  out->data[4] = +roa_float3_get_y(x_axis);
  out->data[5] = +roa_float3_get_y(y_axis);
  out->data[6] = +roa_float3_get_y(z_axis);
  out->data[7] = 0.f;

  out->data[8]  = +roa_float3_get_z(x_axis);
  out->data[9]  = +roa_float3_get_z(y_axis);
  out->data[10] = +roa_float3_get_z(z_axis);
  out->data[11] = 0.f;

  out->data[12] = -roa_float3_dot(x_axis, from);
  out->data[13] = -roa_float3_dot(y_axis, from);
  out->data[14] = -roa_float3_dot(z_axis, from);
  out->data[15] = 1.f;
}


void
roa_mat4_multiply(roa_mat4 *out, roa_mat4 *a, roa_mat4 *b)
{
  assert(out);
  assert(a);
  assert(b);

  for (int i = 0; i < MATH_ARR_COUNT(out->data); ++i)
  {
    /* need dot products */
  }
}


void
roa_mat4_multiply_three(roa_mat4 *, roa_mat4 *, roa_mat4 *, roa_mat4 *)
{
  
}


/* -------------------------------------------------------------- [ ray ] -- */
/* ------------------------------------------------------------- [ aabb ] -- */

/* ---------------------------------------------------------- [ helpers ] -- */


#undef MATH_ARR_COUNT