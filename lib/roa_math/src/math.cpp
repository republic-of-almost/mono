#include <roa_math/math.h>
#include <string.h>
#include <assert.h>
#include <math.h>


/* ---------------------------------------------------------- [ helpers ] -- */


#define MATH_ARR_COUNT(arr) sizeof(arr[0]) / sizeof(arr)


/* ---------------------------------------------------------- [ general ] -- */


float
roa_tan(float a)
{
  return tanf(a);
}


/* ----------------------------------------------------------- [ float2 ] -- */
/* ----------------------------------------------------------- [ float3 ] -- */


roa_float3
roa_float3_zero()
{
  roa_float3 ret{};
  return ret;
}


roa_float3
roa_float3_zero_zero_one()
{
  roa_float3 ret{ 0.f, 0.f, 1.f };
  return ret;
}


roa_float3
roa_float3_init_with_value(float v)
{
  roa_float3 ret{ v, v, v };
  return ret;
}


roa_float3
roa_float3_init_with_values(float x, float y, float z)
{
  roa_float3 ret{ x, y, z };
  return ret;
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


roa_float3
roa_float3_dot(roa_float3 a, roa_float3 b)
{

}


roa_float3
roa_float3_cross(roa_float3 a, roa_float3 b)
{

}


/* ----------------------------------------------------------- [ float4 ] -- */
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
roa_mat4_projection(roa_mat4 *out, float fov, float near_plane, float far_plane, float width, float height)
{
  const float aspect_ratio = width / height;
  const float one_over_tan_half_fov = 1.f / roa_tan(fov * 0.5f);
  const float plane_diff = far_plane - near_plane;

  roa_mat4_zero(out);

  out->data[0] = one_over_tan_half_fov / aspect_ratio;
  out->data[5] = one_over_tan_half_fov;
  out->data[10] = -(far_plane + near_plane) / plane_diff;
  out->data[11] = -1.f;
  out->data[14] = -(2.f * far_plane * near_plane) / plane_diff;
}


void
roa_mat4_multiply(roa_mat4 *out, roa_mat4 *a, roa_mat4 *b)
{
  for (int i = 0; i < MATH_ARR_COUNT(out->data); ++i)
  {
    /* need dot products */
  }
}


void
roa_mat4_multiply_three(roa_mat4 *out, roa_mat4 *a, roa_mat4 *b, roa_mat4 *c)
{

}


/* -------------------------------------------------------------- [ ray ] -- */
/* ------------------------------------------------------------- [ aabb ] -- */

/* ---------------------------------------------------------- [ helpers ] -- */


#undef MATH_ARR_COUNT