#ifndef MATRIX33_INCLUDED_D6DCE389_FA7C_4C48_A303_C0EDC9A90C8B
#define MATRIX33_INCLUDED_D6DCE389_FA7C_4C48_A303_C0EDC9A90C8B


#include "../detail/detail.hpp"
#include "mat_types.hpp"
#include "../vec/vec3.hpp"
#include "../vec/vec2.hpp"
#include <string.h>


// --------------------------------------------------------------- [ Config ] --
/*
  The inline for matrix functions
*/


#define MATH_MAT3_INLINE inline


_MATH_NS_OPEN


// ------------------------------------------------------------ [ Constants ] --
/*
  Common matrix constants
*/


MATH_MAT3_INLINE mat3          mat3_id();
MATH_MAT3_INLINE mat3          mat3_zero();


// ----------------------------------------------------------- [ Initialize ] --
/*
  Initialize a mat3, an mat3_init() will return an identity matrix.
  Perhaps these are the wrong defaults, but need some data to show that.
*/


MATH_MAT3_INLINE mat3          mat3_init(); // will return an id array
MATH_MAT3_INLINE mat3          mat3_init(const float x);
MATH_MAT3_INLINE mat3          mat3_init(const float arr[]);


// ----------------------------------------------- [ Special Init With Data ] --
/*
  These return some specific to 2D/3D rendering mats. Scale Rotate and Translate.
*/


MATH_MAT3_INLINE mat3          mat3_scale(const float x, const float y);
MATH_MAT3_INLINE mat3          mat3_scale(const vec2 scale);
MATH_MAT3_INLINE mat3          mat3_translate(const vec2 vec); // no imp
MATH_MAT3_INLINE mat3          mat3_translate(const float x, const float y); // no imp

MATH_MAT3_INLINE mat3          mat3_rotation_pitch_from_euler(const float radians);
MATH_MAT3_INLINE mat3          mat3_rotation_yaw_from_euler(const float radians);
MATH_MAT3_INLINE mat3          mat3_rotation_roll_from_euler(const float radians);


// ----------------------------------------------------------- [ Operations ] --


MATH_MAT3_INLINE mat3          mat3_add(const mat3 &lhs, const mat3 &rhs);
MATH_MAT3_INLINE mat3          mat3_subtract(const mat3 &lhs, const mat3 &rhs);

MATH_MAT3_INLINE vec3          mat3_multiply(const vec3 vec, const mat3 &b);
MATH_MAT3_INLINE mat3          mat3_multiply(const mat3 &lhs, const mat3 &rhs);
MATH_MAT3_INLINE mat3          mat3_multiply(const mat3 &a, const mat3 &b, const mat3 &c);
MATH_MAT3_INLINE mat3          mat3_scale(const mat3 &a, const vec3 scale); // ?? what does this do

MATH_MAT3_INLINE mat3          mat3_transpose(const mat3 &a);
MATH_MAT3_INLINE mat3          mat3_inverse(const mat3 &a);
MATH_MAT3_INLINE float         mat3_determinant(const mat3 &a);


// ----------------------------------------------------------------- [ Data ] --
/*
  Utility methods, for getting and setting data.
*/


MATH_MAT3_INLINE void          mat3_to_array(const mat3 &m, float *array);
MATH_MAT3_INLINE float         mat3_get(const mat3 &mat, const uint32_t row, const uint32_t col);
MATH_MAT3_INLINE float         mat3_get(const mat3 &mat, const uint32_t i);
MATH_MAT3_INLINE void          mat3_set(mat3 &mat, const uint32_t row, const uint32_t col, const float set);
MATH_MAT3_INLINE vec3          mat3_get_scale(const mat3 &a); // ??


// ------------------------------------------------------------- [ Equality ] --
/*
  Mat operators.
*/


MATH_MAT3_INLINE bool          mat3_equal(const mat3 &left, const mat3 &right, const float error = MATH_NS_NAME::epsilon());


// --------------------------------------------------------------- [ Config ] --


#undef MATH_MAT3_INLINE


// ------------------------------------------------------- [ Constants Impl ] --


mat3
mat3_id()
{
  const float id_array[9] = {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
  };

  return mat3_init(id_array);
}


mat3
mat3_zero()
{
  const float zero_array[9] = {
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
  };

  return mat3_init(zero_array);
}


// ------------------------------------------------------ [ Initialize Impl ] --


mat3
mat3_init()
{
  return mat3_id();
}


mat3
mat3_init(const float x)
{
  float array[9];
  std::fill_n(array, 9, x);

  return mat3_init(array);
}


mat3
mat3_init(const float array[])
{
  mat3 return_mat;
  memcpy(return_mat.data, array, sizeof(return_mat.data));

  return return_mat;
}


// ------------------------------------------------------ [ Operations Impl ] --


mat3
mat3_add(const mat3 &lhs, const mat3 &rhs)
{
  mat3 return_mat;

  for(uint32_t i = 0; i < 9; ++i)
  {
    return_mat.data[i] = lhs.data[0] + rhs.data[0];
  }

  return return_mat;
}


mat3
mat3_subtract(const mat3 &lhs, const mat3 &rhs)
{
  mat3 return_mat;

  for(uint32_t i = 0; i < 9; ++i)
  {
    return_mat.data[i] = lhs.data[0] - rhs.data[0];
  }

  return return_mat;
}


mat3
mat3_scale(const vec2 scale)
{
  return mat3_scale(vec2_get_x(scale), vec2_get_y(scale));
}


mat3
mat3_scale(const float x, const float y)
{
  mat3 return_mat = mat3_id();

  return_mat.data[0] = x;
  return_mat.data[4] = y;

  return return_mat;
}


vec3
mat3_multiply(const vec3 lhs, const mat3 &rhs)
{
  ALIGN16 float vec_data[3];

  for(uint32_t i = 0; i < 3; ++i)
  {
    const vec3 dot_vec = vec3_init(
      rhs.data[i + 0],
      rhs.data[i + 3],
      rhs.data[i + 6]
    );

    vec_data[i] = vec3_dot(lhs, dot_vec);
  }

  return vec3_init(vec_data);
}


mat3
mat3_multiply(const mat3 &lhs, const mat3 &rhs)
{
  mat3 return_mat = mat3_id();

  for(uint32_t i = 0; i < 9; ++i)
  {
    //[0,1,2,3] x [0,4,8,12]
    const uint32_t row = (i / 3) * 3;
    const uint32_t col = i % 3;

    const vec3 left_vec = vec3_init(
      lhs.data[row + 0],
      lhs.data[row + 1],
      lhs.data[row + 2]
    );

    const vec3 right_vec = vec3_init(
      rhs.data[col + 0],
      rhs.data[col + 3],
      rhs.data[col + 6]
    );

    const float dot = vec3_dot(left_vec, right_vec);
    return_mat.data[i] = dot;
  }

  return return_mat;
}


mat3
mat3_multiply(const mat3 &a, const mat3 &b, const mat3 &c)
{
  return mat3_multiply(a, mat3_multiply(b, c));
}


mat3
mat3_rotation_pitch_from_euler(const float rad)
{
  const float array[9]
  {
    1.f, 0.f, 0.f,
    0.f, MATH_NS_NAME::cos(rad), MATH_NS_NAME::sin(rad),
    0.f, -MATH_NS_NAME::sin(rad), MATH_NS_NAME::cos(rad),
  };

  return mat3_init(array);
}


mat3
mat3_rotation_yaw_from_euler(const float rad)
{
  const float array[9]
  {
    MATH_NS_NAME::cos(rad), 0.f, -MATH_NS_NAME::sin(rad),
    0.f, 1.f, 0.f,
    MATH_NS_NAME::sin(rad), 0.f, MATH_NS_NAME::cos(rad),
  };

  return mat3_init(array);
}


mat3
mat3_rotation_roll_from_euler(const float rad)
{
  const float array[9]
  {
    MATH_NS_NAME::cos(rad), MATH_NS_NAME::sin(rad), 0.f,
    -MATH_NS_NAME::sin(rad), MATH_NS_NAME::cos(rad), 0.f,
    0.f, 0.f, 1.f,
  };

  return mat3_init(array);
}


float
mat3_determinant(const mat3 &det)
{
  /*
    | i j k |   | 0 1 2 |
    | a b c | = | 3 4 5 |
    | d e f |   | 6 7 8 |
  */

  const float i = det.data[0];
  const float j = det.data[1];
  const float k = det.data[2];

  const float a = det.data[3];
  const float b = det.data[4];
  const float c = det.data[5];

  const float d = det.data[6];
  const float e = det.data[7];
  const float f = det.data[8];

  return (i * (b * f - c * e)) +
         (j * (a * f - c * d)) -
         (k * (a * e - b * d));
}


mat3
mat3_transpose(const mat3 &m)
{
  const float transpose_data[9] = {
    m.data[0], m.data[3], m.data[6],
    m.data[1], m.data[4], m.data[7],
    m.data[2], m.data[5], m.data[8],
  };

  return mat3_init(transpose_data);
}


void
mat3_to_array(const mat3 &m, float out_data[9])
{
  memcpy(out_data, m.data, sizeof(float) * 9);
}


// -------------------------------------------------------- [ Equality Impl ] --


bool
mat3_equal(const mat3 &left,
           const mat3 &right,
           const float error)
{
  float a[9];
  mat3_to_array(left, a);

  float b[9];
  mat3_to_array(right, b);

  for(uint32_t i = 0; i < 9; ++i)
  {
    if(!MATH_NS_NAME::is_near(a[i], b[i], error))
    {
      return false;
    }
  }

  return true;
}


// ------------------------------------------------------- [ Get / Set Impl ] --


float
mat3_get(const mat3 &mat, const uint32_t row, const uint32_t col)
{
  assert(row < 4 && col < 4);

  const uint32_t i = (row * 3) + col;
  return mat3_get(mat, i);
}


float
mat3_get(const mat3 &mat, const uint32_t index)
{
  assert(index < 9);

  return mat.data[index];
}


void
mat3_set(mat3 &mat, const uint32_t row, const uint32_t col, const float set)
{
  assert(row < 4 && col < 4);

  mat.data[(row * 4) + col] = set;
}


_MATH_NS_CLOSE


#endif // include guard
