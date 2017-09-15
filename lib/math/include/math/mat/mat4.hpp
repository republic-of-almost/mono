#ifndef MATRIX44_INCLUDED_45952EA8_98A0_40E4_8677_AE79A577E975
#define MATRIX44_INCLUDED_45952EA8_98A0_40E4_8677_AE79A577E975


#include "../detail/detail.hpp"
#include "mat_types.hpp"
#include "mat3.hpp"
#include "../vec/vec4.hpp"
#include <string.h>


// --------------------------------------------------------------- [ Config ] --
/*
  The inline for matrix functions
*/


#define MATH_MAT4_INLINE inline


_MATH_NS_OPEN


// ------------------------------------------------------------ [ Constants ] --
/*
  Common matrix constants.
*/


MATH_MAT4_INLINE mat4         mat4_id();
MATH_MAT4_INLINE mat4         mat4_zero();


// ----------------------------------------------------------- [ Initialize ] --
/*
  Initialize a mat4, an mat4_init() will return an identity matrix. Init with a
  mat3 will have the last component as 1.f. Perhaps these are the wrong
  defaults, but need some data to show that.
*/


MATH_MAT4_INLINE mat4         mat4_init();
MATH_MAT4_INLINE mat4         mat4_init(const float x);
MATH_MAT4_INLINE mat4         mat4_init(const mat3 sub_matrix);
MATH_MAT4_INLINE mat4         mat4_init(const float arr[]);


// ----------------------------------------------- [ Special Init With Data ] --
/*
  These return some specific to 3D rendering mats. Scale Rotate and Translate.
*/


MATH_MAT4_INLINE mat4         mat4_scale(const vec3 scale);
MATH_MAT4_INLINE mat4         mat4_scale(const float x, const float y, const float z);
MATH_MAT4_INLINE mat4         mat4_translate(const vec3 move);
MATH_MAT4_INLINE mat4         mat4_translate(const float x, const float y, const float z);
MATH_MAT4_INLINE mat4                       mat4_rotate_around_axis(const vec3 axis, const float radians);


// ---------------------------------------------------------- [ Camera Mats ] --
/*
  Common camera matrices, where lookat is the inverse camera's world mat.
  and perspective and orthographics are projection mats.
*/


MATH_MAT4_INLINE mat4         mat4_lookat(const vec3 eye_position, const vec3 look_at_position, const vec3 up);
MATH_MAT4_INLINE mat4         mat4_perspective(const float width, const float height, const float near_plane, const float far_plane, const float fov);
MATH_MAT4_INLINE mat4         mat4_orthographic(const float width, const float height, const float near_plane,  const float far_plane);


// ----------------------------------------------------------- [ Operations ] --
/*
  General mat math.
*/


MATH_MAT4_INLINE mat4         mat4_add(const mat4 &lhs, const mat4 &rhs);
MATH_MAT4_INLINE mat4         mat4_subtract(const mat4 &lhs, const mat4 &rhs);

MATH_MAT4_INLINE mat4         mat4_multiply(const float val, const mat4 &b);
MATH_MAT4_INLINE vec4         mat4_multiply(const vec4 vec, const mat4 &b);
MATH_MAT4_INLINE mat4         mat4_multiply(const mat4 &lhs, const mat4 &rhs);
MATH_MAT4_INLINE mat4         mat4_multiply(const mat4 &one, const mat4 &two, const mat4 &three);

MATH_MAT4_INLINE mat4         mat4_transpose(const mat4 &a);
MATH_MAT4_INLINE mat4         mat4_inverse(const mat4 &a);
MATH_MAT4_INLINE float        mat4_get_determinant(const mat4 &a);


// ----------------------------------------------------------------- [ Data ] --
/*
  Utility methods, for getting and setting data.
*/


MATH_MAT4_INLINE mat3         mat4_get_sub_mat3(const mat4 &a);
MATH_MAT4_INLINE const float* mat4_data(const mat4 &mat);
MATH_MAT4_INLINE void         mat4_to_array(const mat4 &m, float *array);


// ------------------------------------------------------------- [ Equality ] --
/*
  Mat operators.
*/


MATH_MAT4_INLINE bool         mat4_is_not_near(const mat4 &a, const mat4 &b, const float error = MATH_NS_NAME::epsilon());
MATH_MAT4_INLINE bool         mat4_is_near(const mat4 &a, const mat4 &b, const float error = MATH_NS_NAME::epsilon());


// --------------------------------------------------------------- [ Config ] --


#undef MATH_MAT4_INLINE


// ------------------------------------------------------------ [ Constants ] --


mat4
mat4_id()
{
  const float id_array[16] = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f,
  };

  return mat4_init(id_array);
}


mat4
mat4_zero()
{
  const float zero_array[16] = {
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
    0.f, 0.f, 0.f, 0.f,
  };

  return mat4_init(zero_array);
}


// ----------------------------------------------------------- [ Initialize ] --


mat4
mat4_init()
{
  return mat4_id();
}


mat4
mat4_init(const float x)
{
  float array[16];

  for(float &i : array)
  {
    i = x;
  }

  return mat4_init(array);
}


mat4
mat4_init(const mat3 sub_matrix)
{
  const float mat_data[16] = {
    mat3_get(sub_matrix, 0), mat3_get(sub_matrix, 1), mat3_get(sub_matrix, 2), 0.f,
    mat3_get(sub_matrix, 3), mat3_get(sub_matrix, 4), mat3_get(sub_matrix, 5), 0.f,
    mat3_get(sub_matrix, 6), mat3_get(sub_matrix, 7), mat3_get(sub_matrix, 8), 0.f,
    0.f, 0.f, 0.f, 0.f,
  };

  return mat4_init(mat_data);
}


mat4
mat4_init(const float array[])
{
  mat4 return_mat;
  memcpy(return_mat.data, array, sizeof(return_mat.data));

  return return_mat;
}


// ----------------------------------------------- [ Special Init With Data ] --


mat4
mat4_scale(const vec3 scale_vec)
{
  return mat4_scale(vec3_get_x(scale_vec), vec3_get_y(scale_vec), vec3_get_z(scale_vec));
}


mat4
mat4_scale(const float x, const float y, const float z)
{
  mat4 return_mat = mat4_id();

  return_mat.data[0]  = x;
  return_mat.data[5]  = y;
  return_mat.data[10] = z;

  return return_mat;
}


mat4
mat4_translate(const vec3 move)
{
  return MATH_NS_NAME::mat4_translate(
    MATH_NS_NAME::vec3_get_x(move),
    MATH_NS_NAME::vec3_get_y(move),
    MATH_NS_NAME::vec3_get_z(move)
  );
}


mat4
mat4_translate(const float x, const float y, const float z)
{
  MATH_NS_NAME::mat4 translate_mat = MATH_NS_NAME::mat4_id();

  translate_mat.data[12] += x;
  translate_mat.data[13] += y;
  translate_mat.data[14] += z;

  return translate_mat;
}


mat4
mat4_rotate_around_axis(const vec3 axis, const float radians)
{
  MATH_NS_NAME::mat4 rotation = MATH_NS_NAME::mat4_id();

  const MATH_NS_NAME::vec3 norm_axis = MATH_NS_NAME::vec3_normalize(axis);
  MATH_ASSERT(MATH_NS_NAME::vec3_length(norm_axis));

  const float sin_theta           = MATH_NS_NAME::sin(radians);
  const float cos_theta           = MATH_NS_NAME::cos(radians);
  const float one_minus_cos_theta = 1.f - cos_theta;

  const float x = MATH_NS_NAME::vec3_get_x(norm_axis);
  const float y = MATH_NS_NAME::vec3_get_y(norm_axis);
  const float z = MATH_NS_NAME::vec3_get_z(norm_axis);

  rotation.data[0]  = cos_theta + ((x * x) * one_minus_cos_theta);
  rotation.data[1]  = ((x * y) * one_minus_cos_theta) - (z * sin_theta);
  rotation.data[2]  = ((x * z) * one_minus_cos_theta) + (y * sin_theta);
  rotation.data[3]  = 0;

  rotation.data[4]  = ((y * x) * one_minus_cos_theta) + (z * sin_theta);
  rotation.data[5]  = cos_theta + ((y * y) * one_minus_cos_theta);
  rotation.data[6]  = ((y * z) * one_minus_cos_theta) - (x * sin_theta);
  rotation.data[7]  = 0;

  rotation.data[8]  = ((z * x) * one_minus_cos_theta) - (y * sin_theta);
  rotation.data[9]  = ((z * y) * one_minus_cos_theta) + (x * sin_theta);
  rotation.data[10] = cos_theta + ((z * z) * one_minus_cos_theta);
  rotation.data[11] = 0;

  rotation.data[12] = 0;
  rotation.data[13] = 0;
  rotation.data[14] = 0;
  rotation.data[15] = 1;

  return rotation;
}


// ---------------------------------------------------------- [ Camera Mats ] --


mat4
mat4_lookat(const vec3 eye_position, const vec3 look_at_position, const vec3 up)
{
  const MATH_NS_NAME::vec3 z_axis = MATH_NS_NAME::vec3_normalize(vec3_subtract(look_at_position, eye_position));
  const MATH_NS_NAME::vec3 x_axis = MATH_NS_NAME::vec3_normalize(vec3_cross(z_axis, up));
  const MATH_NS_NAME::vec3 y_axis = MATH_NS_NAME::vec3_cross(x_axis, z_axis);

  const float array_mat[16] =
  {
    +MATH_NS_NAME::vec3_get_x(x_axis),
    +MATH_NS_NAME::vec3_get_x(y_axis),
    -MATH_NS_NAME::vec3_get_x(z_axis),
    0.f,

    +MATH_NS_NAME::vec3_get_y(x_axis),
    +MATH_NS_NAME::vec3_get_y(y_axis),
    -MATH_NS_NAME::vec3_get_y(z_axis),
    0.f,

    +MATH_NS_NAME::vec3_get_z(x_axis),
    +MATH_NS_NAME::vec3_get_z(y_axis),
    -MATH_NS_NAME::vec3_get_z(z_axis),
    0.f,

    -(MATH_NS_NAME::vec3_dot(x_axis, eye_position)),
    -(MATH_NS_NAME::vec3_dot(y_axis, eye_position)),
    +(MATH_NS_NAME::vec3_dot(z_axis, eye_position)),
    1.f,
  };

  return MATH_NS_NAME::mat4_init(array_mat);
}


mat4
mat4_perspective(const float width, const float height, const float near_plane, const float far_plane, const float fov)
{
  const float aspect_ratio = width / height;
  const float one_over_tan_half_fov = 1.f / MATH_NS_NAME::tan(fov * 0.5f);
  const float plane_diff = far_plane - near_plane;

  //mat4 return_mat = mat4_zero();
  const float proj_mat[16] =
  {
    one_over_tan_half_fov / aspect_ratio,
    0.f,
    0.f,
    0.f,

    0.f,
    one_over_tan_half_fov,
    0.f,
    0.f,

    0.f,
    0.f,
    -(far_plane + near_plane) / plane_diff,
    -1.f,

    0.f,
    0.f,
    -(2.f * far_plane * near_plane) / plane_diff,
    0.f,

 };

 return MATH_NS_NAME::mat4_init(proj_mat);
}


mat4
mat4_orthographic(const float width, const float height, const float near_plane, const float far_plane)
{
  const float depth = far_plane - near_plane;
  MATH_ASSERT(depth > 0);

  const float ortho_mat[16] =
  {
    2.f / width,
    0.f,
    0.f,
    0.f,

    0.f,
    2.f / height,
    0.f,
    0.f,

    0.f,
    0.f,
    2.f / depth,
    0.f,//-near_plane / depth,

    0.f,
    0.f,
    0.f,
    1.f,
  };

  return MATH_NS_NAME::mat4_init(ortho_mat);
}


// ----------------------------------------------------------- [ Operations ] --


mat4
mat4_add(const mat4 &lhs, const mat4 &rhs)
{
  MATH_NS_NAME::mat4 return_mat;

  for(int i = 0; i < 16; ++i)
  {
    return_mat.data[i] = lhs.data[0] + rhs.data[0];
  }

  return return_mat;
}


mat4
mat4_subtract(const mat4 &lhs, const mat4 &rhs)
{
  mat4 return_mat;

  for(uint32_t i = 0; i < 16; ++i)
  {
    return_mat.data[i] = lhs.data[0] - rhs.data[0];
  }

  return return_mat;
}


mat4
mat4_multiply(const float lhs, const mat4 &rhs)
{
  mat4 multiply_mat = mat4_init(lhs);

  for(uint32_t i = 0; i < 16; ++i)
  {
    multiply_mat.data[i] = rhs.data[i] * lhs;
  }

  return multiply_mat;
}


vec4
mat4_multiply(const vec4 lhs, const mat4 &rhs)
{
  float result[4]{};

  for(uint32_t i = 0; i < 4; ++i)
  {
    const vec4 dot_vec = MATH_NS_NAME::vec4_init(
      rhs.data[i + 0],
      rhs.data[i + 4],
      rhs.data[i + 8],
      rhs.data[i + 12]
    );

    result[i] = MATH_NS_NAME::vec4_dot(lhs, dot_vec);
  }

  return MATH_NS_NAME::vec4_init_with_array(result);
}


mat4
mat4_multiply(const mat4 &one, const mat4 &two, const mat4 &three)
{
  return MATH_NS_NAME::mat4_multiply(
    MATH_NS_NAME::mat4_multiply(one, two),
    three
  );
}


mat4
mat4_multiply(const mat4 &lhs, const mat4 &rhs)
{
  MATH_NS_NAME::mat4 return_mat = MATH_NS_NAME::mat4_id();

  for(uint32_t i = 0; i < 16; ++i)
  {
    // Starting index for data.
    const uint32_t row = (i / 4) * 4;
    const uint32_t col = (i % 4);

    const vec4 row_vec = MATH_NS_NAME::vec4_init(
      lhs.data[row + 0],
      lhs.data[row + 1],
      lhs.data[row + 2],
      lhs.data[row + 3]
    );

    const vec4 col_vec = MATH_NS_NAME::vec4_init(
      rhs.data[col + 0],
      rhs.data[col + 4],
      rhs.data[col + 8],
      rhs.data[col + 12]
    );

    const float dot = MATH_NS_NAME::vec4_dot(row_vec, col_vec);

    return_mat.data[i] = dot;
  }

  return return_mat;
}


mat4
mat4_transpose(const mat4 &to_transpose)
{
  const float mat_transpose[16]
  {
    to_transpose.data[0],  to_transpose.data[4],  to_transpose.data[8],  to_transpose.data[12],
    to_transpose.data[1],  to_transpose.data[5],  to_transpose.data[9],  to_transpose.data[13],
    to_transpose.data[2],  to_transpose.data[6],  to_transpose.data[10], to_transpose.data[14],
    to_transpose.data[3],  to_transpose.data[7],  to_transpose.data[11], to_transpose.data[15],
  };

  return MATH_NS_NAME::mat4_init(mat_transpose);
}


mat4
mat4_inverse(const mat4 &to_inverse)
{
  const float temp[16]
  {
    // 0
    to_inverse.data[5]  * to_inverse.data[10] * to_inverse.data[15] -
    to_inverse.data[5]  * to_inverse.data[11] * to_inverse.data[14] -
    to_inverse.data[9]  * to_inverse.data[6]  * to_inverse.data[15] +
    to_inverse.data[9]  * to_inverse.data[7]  * to_inverse.data[14] +
    to_inverse.data[13] * to_inverse.data[6]  * to_inverse.data[11] -
    to_inverse.data[13] * to_inverse.data[7]  * to_inverse.data[10],

    // 1
    -to_inverse.data[1]  * to_inverse.data[10] * to_inverse.data[15] +
    to_inverse.data[1]  * to_inverse.data[11] * to_inverse.data[14] +
    to_inverse.data[9]  * to_inverse.data[2] * to_inverse.data[15] -
    to_inverse.data[9]  * to_inverse.data[3] * to_inverse.data[14] -
    to_inverse.data[13] * to_inverse.data[2] * to_inverse.data[11] +
    to_inverse.data[13] * to_inverse.data[3] * to_inverse.data[10],

    // 2
    to_inverse.data[1]  * to_inverse.data[6] * to_inverse.data[15] -
    to_inverse.data[1]  * to_inverse.data[7] * to_inverse.data[14] -
    to_inverse.data[5]  * to_inverse.data[2] * to_inverse.data[15] +
    to_inverse.data[5]  * to_inverse.data[3] * to_inverse.data[14] +
    to_inverse.data[13] * to_inverse.data[2] * to_inverse.data[7] -
    to_inverse.data[13] * to_inverse.data[3] * to_inverse.data[6],

    // 3
    -to_inverse.data[1] * to_inverse.data[6] * to_inverse.data[11] +
    to_inverse.data[1] * to_inverse.data[7] * to_inverse.data[10] +
    to_inverse.data[5] * to_inverse.data[2] * to_inverse.data[11] -
    to_inverse.data[5] * to_inverse.data[3] * to_inverse.data[10] -
    to_inverse.data[9] * to_inverse.data[2] * to_inverse.data[7] +
    to_inverse.data[9] * to_inverse.data[3] * to_inverse.data[6],

    // 4
    -to_inverse.data[4]  * to_inverse.data[10] * to_inverse.data[15] +
    to_inverse.data[4]  * to_inverse.data[11] * to_inverse.data[14] +
    to_inverse.data[8]  * to_inverse.data[6]  * to_inverse.data[15] -
    to_inverse.data[8]  * to_inverse.data[7]  * to_inverse.data[14] -
    to_inverse.data[12] * to_inverse.data[6]  * to_inverse.data[11] +
    to_inverse.data[12] * to_inverse.data[7]  * to_inverse.data[10],

    // 5
    to_inverse.data[0]  * to_inverse.data[10] * to_inverse.data[15] -
    to_inverse.data[0]  * to_inverse.data[11] * to_inverse.data[14] -
    to_inverse.data[8]  * to_inverse.data[2] * to_inverse.data[15] +
    to_inverse.data[8]  * to_inverse.data[3] * to_inverse.data[14] +
    to_inverse.data[12] * to_inverse.data[2] * to_inverse.data[11] -
    to_inverse.data[12] * to_inverse.data[3] * to_inverse.data[10],

    // 6
    -to_inverse.data[0]  * to_inverse.data[6] * to_inverse.data[15] +
    to_inverse.data[0]  * to_inverse.data[7] * to_inverse.data[14] +
    to_inverse.data[4]  * to_inverse.data[2] * to_inverse.data[15] -
    to_inverse.data[4]  * to_inverse.data[3] * to_inverse.data[14] -
    to_inverse.data[12] * to_inverse.data[2] * to_inverse.data[7] +
    to_inverse.data[12] * to_inverse.data[3] * to_inverse.data[6],

    // 7
    to_inverse.data[0] * to_inverse.data[6] * to_inverse.data[11] -
    to_inverse.data[0] * to_inverse.data[7] * to_inverse.data[10] -
    to_inverse.data[4] * to_inverse.data[2] * to_inverse.data[11] +
    to_inverse.data[4] * to_inverse.data[3] * to_inverse.data[10] +
    to_inverse.data[8] * to_inverse.data[2] * to_inverse.data[7] -
    to_inverse.data[8] * to_inverse.data[3] * to_inverse.data[6],

    // 8
    to_inverse.data[4]  * to_inverse.data[9] * to_inverse.data[15] -
    to_inverse.data[4]  * to_inverse.data[11] * to_inverse.data[13] -
    to_inverse.data[8]  * to_inverse.data[5] * to_inverse.data[15] +
    to_inverse.data[8]  * to_inverse.data[7] * to_inverse.data[13] +
    to_inverse.data[12] * to_inverse.data[5] * to_inverse.data[11] -
    to_inverse.data[12] * to_inverse.data[7] * to_inverse.data[9],

    // 9
    -to_inverse.data[0]  * to_inverse.data[9] * to_inverse.data[15] +
    to_inverse.data[0]  * to_inverse.data[11] * to_inverse.data[13] +
    to_inverse.data[8]  * to_inverse.data[1] * to_inverse.data[15] -
    to_inverse.data[8]  * to_inverse.data[3] * to_inverse.data[13] -
    to_inverse.data[12] * to_inverse.data[1] * to_inverse.data[11] +
    to_inverse.data[12] * to_inverse.data[3] * to_inverse.data[9],

    // 10
    to_inverse.data[0]  * to_inverse.data[5] * to_inverse.data[15] -
    to_inverse.data[0]  * to_inverse.data[7] * to_inverse.data[13] -
    to_inverse.data[4]  * to_inverse.data[1] * to_inverse.data[15] +
    to_inverse.data[4]  * to_inverse.data[3] * to_inverse.data[13] +
    to_inverse.data[12] * to_inverse.data[1] * to_inverse.data[7] -
    to_inverse.data[12] * to_inverse.data[3] * to_inverse.data[5],

    // 11
    -to_inverse.data[0] * to_inverse.data[5] * to_inverse.data[11] +
    to_inverse.data[0] * to_inverse.data[7] * to_inverse.data[9] +
    to_inverse.data[4] * to_inverse.data[1] * to_inverse.data[11] -
    to_inverse.data[4] * to_inverse.data[3] * to_inverse.data[9] -
    to_inverse.data[8] * to_inverse.data[1] * to_inverse.data[7] +
    to_inverse.data[8] * to_inverse.data[3] * to_inverse.data[5],

    // 12
    -to_inverse.data[4]  * to_inverse.data[9] * to_inverse.data[14] +
    to_inverse.data[4]  * to_inverse.data[10] * to_inverse.data[13] +
    to_inverse.data[8]  * to_inverse.data[5] * to_inverse.data[14] -
    to_inverse.data[8]  * to_inverse.data[6] * to_inverse.data[13] -
    to_inverse.data[12] * to_inverse.data[5] * to_inverse.data[10] +
    to_inverse.data[12] * to_inverse.data[6] * to_inverse.data[9],

    // 13
    to_inverse.data[0]  * to_inverse.data[9] * to_inverse.data[14] -
    to_inverse.data[0]  * to_inverse.data[10] * to_inverse.data[13] -
    to_inverse.data[8]  * to_inverse.data[1] * to_inverse.data[14] +
    to_inverse.data[8]  * to_inverse.data[2] * to_inverse.data[13] +
    to_inverse.data[12] * to_inverse.data[1] * to_inverse.data[10] -
    to_inverse.data[12] * to_inverse.data[2] * to_inverse.data[9],

    // 14
    -to_inverse.data[0]  * to_inverse.data[5] * to_inverse.data[14] +
    to_inverse.data[0]  * to_inverse.data[6] * to_inverse.data[13] +
    to_inverse.data[4]  * to_inverse.data[1] * to_inverse.data[14] -
    to_inverse.data[4]  * to_inverse.data[2] * to_inverse.data[13] -
    to_inverse.data[12] * to_inverse.data[1] * to_inverse.data[6] +
    to_inverse.data[12] * to_inverse.data[2] * to_inverse.data[5],

    // 16
    to_inverse.data[0] * to_inverse.data[5] * to_inverse.data[10] -
    to_inverse.data[0] * to_inverse.data[6] * to_inverse.data[9] -
    to_inverse.data[4] * to_inverse.data[1] * to_inverse.data[10] +
    to_inverse.data[4] * to_inverse.data[2] * to_inverse.data[9] +
    to_inverse.data[8] * to_inverse.data[1] * to_inverse.data[6] -
    to_inverse.data[8] * to_inverse.data[2] * to_inverse.data[5]
  };

  const float determinant = mat4_get_determinant(to_inverse);

  MATH_ASSERT(determinant != 0);

  const float one_over_det = 1.f / determinant;

  float inverse[16]{};

  for(size_t i = 0; i < 16; ++i)
  {
    inverse[i] = temp[i] * one_over_det;
  }

  return mat4_init(inverse);
}


float
mat4_get_determinant(const mat4 &det)
{
  const float temp[16]
  {
    // 0
   det.data[5]  *det.data[10] *det.data[15] -
   det.data[5]  *det.data[11] *det.data[14] -
   det.data[9]  *det.data[6]  *det.data[15] +
   det.data[9]  *det.data[7]  *det.data[14] +
   det.data[13] *det.data[6]  *det.data[11] -
   det.data[13] *det.data[7]  *det.data[10],

    // 1
    -det.data[1]  *det.data[10] *det.data[15] +
   det.data[1]  *det.data[11] *det.data[14] +
   det.data[9]  *det.data[2] *det.data[15] -
   det.data[9]  *det.data[3] *det.data[14] -
   det.data[13] *det.data[2] *det.data[11] +
   det.data[13] *det.data[3] *det.data[10],

    // 2
   det.data[1]  *det.data[6] *det.data[15] -
   det.data[1]  *det.data[7] *det.data[14] -
   det.data[5]  *det.data[2] *det.data[15] +
   det.data[5]  *det.data[3] *det.data[14] +
   det.data[13] *det.data[2] *det.data[7] -
   det.data[13] *det.data[3] *det.data[6],

    // 3
    -det.data[1] *det.data[6] *det.data[11] +
   det.data[1] *det.data[7] *det.data[10] +
   det.data[5] *det.data[2] *det.data[11] -
   det.data[5] *det.data[3] *det.data[10] -
   det.data[9] *det.data[2] *det.data[7] +
   det.data[9] *det.data[3] *det.data[6],

    // 4
    -det.data[4]  *det.data[10] *det.data[15] +
   det.data[4]  *det.data[11] *det.data[14] +
   det.data[8]  *det.data[6]  *det.data[15] -
   det.data[8]  *det.data[7]  *det.data[14] -
   det.data[12] *det.data[6]  *det.data[11] +
   det.data[12] *det.data[7]  *det.data[10],

    // 5
   det.data[0]  *det.data[10] *det.data[15] -
   det.data[0]  *det.data[11] *det.data[14] -
   det.data[8]  *det.data[2] *det.data[15] +
   det.data[8]  *det.data[3] *det.data[14] +
   det.data[12] *det.data[2] *det.data[11] -
   det.data[12] *det.data[3] *det.data[10],

    // 6
    -det.data[0]  *det.data[6] *det.data[15] +
   det.data[0]  *det.data[7] *det.data[14] +
   det.data[4]  *det.data[2] *det.data[15] -
   det.data[4]  *det.data[3] *det.data[14] -
   det.data[12] *det.data[2] *det.data[7] +
   det.data[12] *det.data[3] *det.data[6],

    // 7
   det.data[0] *det.data[6] *det.data[11] -
   det.data[0] *det.data[7] *det.data[10] -
   det.data[4] *det.data[2] *det.data[11] +
   det.data[4] *det.data[3] *det.data[10] +
   det.data[8] *det.data[2] *det.data[7] -
   det.data[8] *det.data[3] *det.data[6],

    // 8
   det.data[4]  *det.data[9] *det.data[15] -
   det.data[4]  *det.data[11] *det.data[13] -
   det.data[8]  *det.data[5] *det.data[15] +
   det.data[8]  *det.data[7] *det.data[13] +
   det.data[12] *det.data[5] *det.data[11] -
   det.data[12] *det.data[7] *det.data[9],

    // 9
    -det.data[0]  *det.data[9] *det.data[15] +
   det.data[0]  *det.data[11] *det.data[13] +
   det.data[8]  *det.data[1] *det.data[15] -
   det.data[8]  *det.data[3] *det.data[13] -
   det.data[12] *det.data[1] *det.data[11] +
   det.data[12] *det.data[3] *det.data[9],

    // 10
   det.data[0]  *det.data[5] *det.data[15] -
   det.data[0]  *det.data[7] *det.data[13] -
   det.data[4]  *det.data[1] *det.data[15] +
   det.data[4]  *det.data[3] *det.data[13] +
   det.data[12] *det.data[1] *det.data[7] -
   det.data[12] *det.data[3] *det.data[5],

    // 11
    -det.data[0] *det.data[5] *det.data[11] +
   det.data[0] *det.data[7] *det.data[9] +
   det.data[4] *det.data[1] *det.data[11] -
   det.data[4] *det.data[3] *det.data[9] -
   det.data[8] *det.data[1] *det.data[7] +
   det.data[8] *det.data[3] *det.data[5],

    // 12
    -det.data[4]  *det.data[9] *det.data[14] +
   det.data[4]  *det.data[10] *det.data[13] +
   det.data[8]  *det.data[5] *det.data[14] -
   det.data[8]  *det.data[6] *det.data[13] -
   det.data[12] *det.data[5] *det.data[10] +
   det.data[12] *det.data[6] *det.data[9],

    // 13
   det.data[0]  *det.data[9] *det.data[14] -
   det.data[0]  *det.data[10] *det.data[13] -
   det.data[8]  *det.data[1] *det.data[14] +
   det.data[8]  *det.data[2] *det.data[13] +
   det.data[12] *det.data[1] *det.data[10] -
   det.data[12] *det.data[2] *det.data[9],

    // 14
    -det.data[0]  *det.data[5] *det.data[14] +
   det.data[0]  *det.data[6] *det.data[13] +
   det.data[4]  *det.data[1] *det.data[14] -
   det.data[4]  *det.data[2] *det.data[13] -
   det.data[12] *det.data[1] *det.data[6] +
   det.data[12] *det.data[2] *det.data[5],

    // 16
   det.data[0] *det.data[5] *det.data[10] -
   det.data[0] *det.data[6] *det.data[9] -
   det.data[4] *det.data[1] *det.data[10] +
   det.data[4] *det.data[2] *det.data[9] +
   det.data[8] *det.data[1] *det.data[6] -
   det.data[8] *det.data[2] *det.data[5]
  };

  const float determinant = det.data[0] * temp[0] +det.data[1] * temp[4] +det.data[2] * temp[8] +det.data[3] * temp[12];

  return determinant;
}


// ----------------------------------------------------------------- [ Data ] --


mat3
mat4_get_sub_mat3(const mat4 &mat)
{
  const float mat_data[9] = {
    mat.data[0], mat.data[1], mat.data[2],
    mat.data[4], mat.data[5], mat.data[6],
    mat.data[8], mat.data[9], mat.data[10],
  };

  return mat3_init(mat_data);
}


const float*
mat4_data(const mat4 &mat)
{
  return &(mat.data[0]);
}


void
mat4_to_array(const mat4 &mat, float *out_array)
{
  memcpy(out_array, mat.data, sizeof(float) * 16);
}


// ------------------------------------------------------------- [ Equality ] --


bool
mat4_is_not_near(const mat4 &a, const mat4 &b, const float error)
{
  return !mat4_is_near(a, b, error);
}


bool
mat4_is_near(const mat4 &a, const mat4 &b, const float error)
{
  for(uint32_t i = 0; i < 16; ++i)
  {
    const float diff = b.data[i] - a.data[i];

    if(MATH_NS_NAME::abs(diff) > error)
    {
      return false;
    }
  }

  return true;
}


_MATH_NS_CLOSE


#endif // include guard
