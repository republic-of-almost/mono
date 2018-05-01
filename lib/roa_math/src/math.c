#include <roa_math/math.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>


/* ----------------------------------------------------------- [ helpers ] -- */


#define MATH_ARR_COUNT(arr) sizeof(arr[0]) / sizeof(arr)


/* ----------------------------------------------------------- [ general ] -- */


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
roa_float_acos(float a)
{
  return acosf(a);
}


float
roa_float_sqrt(float a)
{
  return sqrtf(a);
}


float
roa_float_sign(float a)
{
	return a > 0.f ? 1.f : -1.f;
}


float
roa_float_fract(float a)
{
	/*return a - roa_float_abs(a);*/
	//return roa_float_abs((float)(a - (long)a));
  float f = roa_float_abs(a);
  return roa_float_min(f - roa_float_floor(f), 0x1.fffffep-1f);
}


float
roa_float_round(float a)
{
	return roundf(a);
 /* return (int)(a + (0.5f * roa_float_sign(a)));*/
}


float
roa_float_floor(float a)
{
	return floorf(a);
/*  return roa_float_round(a - 0.5f); */
}


float
roa_float_ceil(float a)
{
	return ceilf(a);
/*  return roa_float_round(a + 0.5f);*/
}


float
roa_float_lerp(float a, float b, float mix)
{
  return  a + (b - a) * mix;
}


float
roa_float_max(float a, float b)
{
  return a > b ? a : b;
}


float
roa_float_min(float a, float b)
{
  return a < b ? a : b;
}


float
roa_float_clamp(float val, float a, float b)
{
  float min = roa_float_min(a, b);
  float max = roa_float_max(a, b);

  return roa_float_max(min, roa_float_min(val, max));
}


float
roa_float_abs(float a)
{
  return fabsf(a);
}


int
roa_float_is_near(float a, float b, float err)
{
  return roa_float_abs(roa_float_abs(b) - roa_float_abs(a)) <= err ? 1 : 0;
}


/* ------------------------------------------------------------ [ float2 ] -- */


roa_float2
roa_float2_zero()
{
	return roa_float2_fill_with_value(0.f);
}


roa_float2
roa_float2_zero_one()
{
	return roa_float2_set_with_values(0.f, 1.f);
}


roa_float2
roa_float2_one()
{
	return roa_float2_fill_with_value(1.f);
}


roa_float2
roa_float2_fill_with_value(float v)
{
	return roa_float2_set_with_values(v, v);
}


roa_float2
roa_float2_set_with_values(float x, float y)
{
	roa_float2 vec;
  vec.x = x;
  vec.y = y;

	return vec;
}


roa_float2
roa_float2_import(const float *in)
{
	roa_float2 vec;
	vec.x = in[0];
	vec.y = in[1];

	return vec;
}


void
roa_float2_export(roa_float2 a, float *out)
{
  out[0] = a.x;
  out[1] = a.y;
}


roa_float2
roa_float2_add(roa_float2 a, roa_float2 b)
{
  return roa_float2_set_with_values(
    a.x + b.x,
    a.y + b.y);
}


roa_float2
roa_float2_subtract(roa_float2 a, roa_float2 b)
{
	return roa_float2_set_with_values(
		a.x - b.x,
		a.y - b.y);
}


roa_float2
roa_float2_multiply(roa_float2 a, roa_float2 b)
{
  return roa_float2_set_with_values(
    a.x * b.x,
    a.y * b.y);
}


roa_float2
roa_float2_divide(roa_float2 a, roa_float2 b)
{
  return roa_float2_set_with_values(
    a.x / b.x,
    a.y / b.y);
}


roa_float2
roa_float2_scale(roa_float2 a, float scale)
{
  return roa_float2_set_with_values(
    a.x * scale,
    a.y * scale);
}


roa_float2
roa_float2_floor(roa_float2 a)
{
  return roa_float2_set_with_values(
    roa_float_floor(a.x),
    roa_float_floor(a.y)
  );
}


roa_float2
roa_float2_fract(roa_float2 a)
{
  return roa_float2_set_with_values(
    roa_float_fract(a.x),
    roa_float_fract(a.y)
  );
}


float
roa_float2_squared_length(roa_float2 a)
{
  return a.x * a.x + a.y * a.y;
}


float
roa_float2_length(roa_float2 a)
{
  return roa_float_sqrt(roa_float2_squared_length(a));
}


float
roa_float2_get_x(roa_float2 a)
{
	return a.x;
}


float
roa_float2_get_y(roa_float2 a)
{
	return a.y;
}


float
roa_float2_dot(roa_float2 a, roa_float2 b)
{
  return (roa_float2_get_x(a) * roa_float2_get_x(b)) +
    (roa_float2_get_y(a) * roa_float2_get_y(b));
}


/* ------------------------------------------------------------ [ float3 ] -- */


roa_float3
roa_float3_zero()
{
  roa_float3 ret = {0.f, 0.f, 0.f};

  return ret;
}


roa_float3
roa_float3_zero_zero_one()
{
  roa_float3 ret;
  ret.x = 0.f;
  ret.y = 0.f;
  ret.z = 1.f;

  return ret;
}


roa_float3
roa_float3_one()
{
  roa_float3 ret;
  ret.x = 1.f;
  ret.y = 1.f;
  ret.z = 1.f;

  return ret;
}

roa_float3
roa_float3_fill_with_value(float v)
{
  roa_float3 ret;
  ret.x = v;
  ret.y = v;
  ret.z = v;

  return ret;
}


roa_float3
roa_float3_set_with_values(float x, float y, float z)
{
  roa_float3 ret;
  ret.x = x;
  ret.y = y;
  ret.z = z;

  return ret;
}


roa_float3
roa_float3_import(const float *in)
{
	roa_float3 ret;
	ret.x = in[0];
	ret.y = in[1];
	ret.z = in[2];

	return ret;
}


void
roa_float3_export(roa_float3 a, float *out)
{
  out[0] = a.x;
  out[1] = a.y;
  out[2] = a.z;
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


roa_float3
roa_float3_lerp(roa_float3 a, roa_float3 b, float t)
{
	roa_float3 c = roa_float3_subtract(b, a);
	roa_float3 offset = roa_float3_scale(c, t);
	roa_float3 result = roa_float3_add(a, offset);

	return result;
}


roa_float3
roa_float3_reflect(roa_float3 v, roa_float3 n)
{
  float scale = 2.f * roa_float3_dot(v, n);
  roa_float3 scaled = roa_float3_scale(n, scale);

  return roa_float3_subtract(v, scaled);
}


float
roa_float3_length(roa_float3 a)
{
  return roa_float_sqrt(roa_float3_squared_length(a));
}


float
roa_float3_squared_length(roa_float3 a)
{
  return a.x * a.x + a.y * a.y + a.z * a.z;
}


roa_float3
roa_float3_normalize(roa_float3 a)
{
  float length = roa_float3_length(a);

  assert(length != 0); /* Don't pass zero vectors. (0,0,0); */

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
  float x = (roa_float3_get_y(a) * roa_float3_get_z(b)) -
						(roa_float3_get_z(a) * roa_float3_get_y(b));

  float y = (roa_float3_get_x(a) * roa_float3_get_z(b)) -
						(roa_float3_get_z(a) * roa_float3_get_x(b));

  float z = (roa_float3_get_x(a) * roa_float3_get_y(b)) -
						(roa_float3_get_y(a) * roa_float3_get_x(b));

  return roa_float3_set_with_values(x, -y, z);
}


roa_float3
roa_float3_scale(roa_float3 a, float scale)
{
  const roa_float3 scale_vec = roa_float3_fill_with_value(scale);
  return roa_float3_multiply(a, scale_vec);
}


roa_float3
roa_float3_fract(roa_float3 a)
{
  return roa_float3_set_with_values(
    roa_float_fract(a.x),
    roa_float_fract(a.y),
    roa_float_fract(a.z));
}


roa_float3
roa_float3_floor(roa_float3 a)
{
  return roa_float3_set_with_values(
    roa_float_floor(a.x),
    roa_float_floor(a.y),
    roa_float_floor(a.z));
}


roa_float3
roa_float3_ceil(roa_float3 a)
{
  return roa_float3_set_with_values(
    roa_float_ceil(a.x),
    roa_float_ceil(a.y),
    roa_float_ceil(a.z));
}


int
roa_float3_is_near(roa_float3 a, roa_float3 b, float err)
{
  int x_is_near = roa_float_is_near(a.x, b.x, err);
  int y_is_near = roa_float_is_near(a.y, b.y, err);
  int z_is_near = roa_float_is_near(a.z, b.z, err);

  return (x_is_near + y_is_near + z_is_near) == 3;
}


/* ------------------------------------------------------------ [ float4 ] -- */


roa_float4
roa_float4_zero()
{
  roa_float4 ret;
  ret.x = 0.f;
  ret.y = 0.f;
  ret.z = 0.f;
  ret.w = 0.f;

  return ret;
}


roa_float4
roa_float4_zero_zero_zero_one()
{
  roa_float4 ret;
  ret.x = 0.f;
  ret.y = 0.f;
  ret.z = 0.f;
  ret.w = 1.f;

  return ret;
}


roa_float4
roa_float4_one()
{
  roa_float4 ret;
  ret.x = 1.f;
  ret.y = 1.f;
  ret.z = 1.f;
  ret.w = 1.f;

  return ret;
}


roa_float4
roa_float4_fill_with_value(float v)
{
  roa_float4 ret;
  ret.x = v;
  ret.y = v;
  ret.z = v;
  ret.w = v;

  return ret;
}


roa_float4
roa_float4_set_with_values(float x, float y, float z, float w)
{
  roa_float4 ret;
  ret.x = x;
  ret.y = y;
  ret.z = z;
  ret.w = w;

  return ret;
}


roa_float4
roa_float4_import(const float *out)
{
	return roa_float4_set_with_values(out[0], out[1], out[2], out[3]);
}


void
roa_float4_export(roa_float4 a, float *out)
{
  out[0] = a.x;
  out[1] = a.y;
  out[2] = a.z;
  out[3] = a.w;
}


float
roa_float4_get_x(roa_float4 a)
{
  return a.x;
}


float
roa_float4_get_y(roa_float4 a)
{
  return a.y;
}


float
roa_float4_get_z(roa_float4 a)
{
  return a.z;
}


float
roa_float4_get_w(roa_float4 a)
{
  return a.w;
}


roa_float4
roa_float4_add(roa_float4 a, roa_float4 b)
{
  roa_float4 ret;
  ret.x = a.x + b.x;
  ret.y = a.y + b.y;
  ret.z = a.z + b.z;
  ret.w = a.w + b.w;

  return ret;
}


roa_float4
roa_float4_subtract(roa_float4 a, roa_float4 b)
{
  roa_float4 ret;
  ret.x = a.x - b.x;
  ret.y = a.y - b.y;
  ret.z = a.z - b.z;
  ret.w = a.w - b.w;

  return ret;
}


roa_float4
roa_float4_multiply(roa_float4 a, roa_float4 b)
{
  roa_float4 ret;
  ret.x = a.x * b.x;
  ret.y = a.y * b.y;
  ret.z = a.z * b.z;
  ret.w = a.w * b.w;

  return ret;
}


roa_float4
roa_float4_lerp(roa_float4 a, roa_float4 b, float t)
{
	roa_float4 c = roa_float4_subtract(b, a);
	roa_float4 offset = roa_float4_scale(c, t);
	roa_float4 result = roa_float4_add(a, offset);

	return result;
}


float
roa_float4_length(roa_float4 a)
{
  float len = (a.x * a.x) +
							(a.y * a.y) +
							(a.z * a.z) +
							(a.w * a.w);

	float result = roa_float_sqrt(len);

  return result;
}


roa_float4
roa_float4_normalize(roa_float4 a)
{
  float length = roa_float4_length(a);

  assert(length != 0); /* Don't pass zero vectors. (0,0,0,0); */

  return roa_float4_scale(a, (1.f / length));

}


float
roa_float4_dot(roa_float4 a, roa_float4 b)
{
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}


roa_float4
roa_float4_scale(roa_float4 a, float scale)
{
  roa_float4 scale_vec = roa_float4_fill_with_value(scale);
  return roa_float4_multiply(a, scale_vec);
}


int
roa_float4_is_near(roa_float4 a, roa_float4 b, float err)
{
  int x_is_near = roa_float_is_near(a.x, b.x, err);
  int y_is_near = roa_float_is_near(a.y, b.y, err);
  int z_is_near = roa_float_is_near(a.z, b.z, err);
  int w_is_near = roa_float_is_near(a.w, b.w, err);

  return (x_is_near + y_is_near + z_is_near + w_is_near) == 4;
}


/* -------------------------------------------------------- [ quaternion ] -- */


roa_quaternion
roa_quaternion_default()
{
  roa_quaternion quat = {
    0.f, 0.f, 0.f, 1.f
  };

  return quat;
}


roa_quaternion
roa_quaternion_import(const float *import_val)
{
  return roa_quaternion_set_with_values(
    import_val[0],
    import_val[1],
    import_val[2],
    import_val[3]);
}


void
roa_quaternion_export(roa_quaternion q, float *export_data)
{
  export_data[0] = q.x;
  export_data[1] = q.y;
  export_data[2] = q.z;
  export_data[3] = q.w;
}

roa_quaternion
roa_quaternion_set_with_values(float x, float y, float z, float w)
{
	roa_float4 vec = roa_float4_set_with_values(x, y, z, w);
	roa_float4 norm = roa_float4_normalize(vec);

  roa_quaternion quat;
  quat.x = norm.x;
  quat.y = norm.y;
  quat.z = norm.z;
  quat.w = norm.w;

  return quat;
}


roa_quaternion
roa_quaternion_from_axis_angle(roa_float3 axis, float angle)
{
  const float half_angle = 0.5f * angle;
  const float sin_angle  = roa_float_sin(half_angle);

  const float qx = sin_angle * axis.x;
  const float qy = sin_angle * axis.y;
  const float qz = sin_angle * axis.z;
  const float qw = roa_float_cos(half_angle);

  return roa_quaternion_set_with_values(qx, qy, qz, qw);
}


roa_quaternion
roa_quaternion_multiply(roa_quaternion left, roa_quaternion right)
{
  float w = (left.w * right.w) -
						(left.x * right.x) -
						(left.y * right.y) -
						(left.z * right.z);

  float x = (left.w * right.x) +
						(left.x * right.w) +
						(left.y * right.z) -
						(left.z * right.y);

  float y = (left.w * right.y) +
						(left.y * right.w) +
						(left.z * right.x) -
						(left.x * right.z);

  float z = (left.w * right.z) +
						(left.z * right.w) +
						(left.x * right.y) -
						(left.y * right.x);

  return roa_quaternion_set_with_values(x, y, z, w);
}


roa_quaternion
roa_quaternion_normalize(roa_quaternion quat)
{
  roa_float4 vec = roa_float4_set_with_values(quat.x, quat.y, quat.z, quat.w);
  roa_float4 nor = roa_float4_normalize(vec);

  return roa_quaternion_set_with_values(nor.x, nor.y, nor.z, nor.w);
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


void
roa_quaternion_get_rotation_matrix(roa_quaternion rot, roa_mat3 *out)
{
	float x_sq = rot.x * rot.x;
  float y_sq = rot.y * rot.y;
  float z_sq = rot.z * rot.z;

  float mat_data[9];
  mat_data[0] = 1 - 2 * y_sq - 2 * z_sq;
  mat_data[1] = 2 * (rot.x * rot.y) + 2 * (rot.z * rot.w);
  mat_data[2] = 2 * (rot.x * rot.z) - 2 * (rot.y * rot.w);

  mat_data[3] = 2 * (rot.x * rot.y) - 2 * (rot.z * rot.w);
  mat_data[4] = 1 - 2 * x_sq - 2 * z_sq;
  mat_data[5] = 2 * (rot.y * rot.z) + 2 * (rot.x * rot.w);

  mat_data[6] = 2 * (rot.x * rot.z) + 2 * (rot.y * rot.w);
  mat_data[7] = 2 * (rot.y * rot.z) - 2 * (rot.x * rot.w);
  mat_data[8] = 1 - 2 * x_sq - 2 * y_sq;

  roa_mat3_import(out, mat_data);
}


roa_float3
roa_quaternion_rotate_vector(roa_quaternion rotation, roa_float3 vector)
{
	roa_mat3 rot_mat;
	roa_quaternion_get_rotation_matrix(rotation, &rot_mat);
	return roa_mat3_multiply_with_float3(vector, &rot_mat);
}


float
roa_quaternion_get_x(roa_quaternion a)
{
	return a.x;
}


float
roa_quaternion_get_y(roa_quaternion a)
{
	return a.y;
}


float
roa_quaternion_get_z(roa_quaternion a)
{
	return a.z;
}


float
roa_quaternion_get_w(roa_quaternion a)
{
	return a.w;
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


void
roa_transform_to_mat4(
  const roa_transform *trans,
  roa_mat4 *out)
{
  /* scale */
  roa_mat4 scale;
  roa_mat4_scale(&scale, trans->scale);

  /* rotation */
  roa_mat4 rotate;
  roa_mat4_rotate(&rotate, trans->rotation);

  /* translation */
  roa_mat4 translate;
  roa_mat4_translate(&translate, trans->position);

  roa_mat4_multiply_three(out, &scale, &rotate, &translate);
}


void
roa_transform_export_mat4(
  const roa_transform *trans,
  float *out)
{
  roa_mat4 to_export;
  roa_transform_to_mat4(trans, &to_export);

  roa_mat4_export(&to_export, out);
}


roa_float3
roa_transform_world_fwd()
{
	return roa_float3_set_with_values(0.f, 0.f, -1.f);
}


roa_float3
roa_transform_local_fwd(const roa_transform *trans)
{
	roa_float3 local_dir = roa_quaternion_rotate_vector(
		trans->rotation,
		roa_transform_world_fwd());

	return local_dir;
}


roa_float3
roa_transform_world_up()
{
	return roa_float3_set_with_values(0.f, +1.f, 0.f);
}


roa_float3
roa_transform_local_up(const roa_transform *trans)
{
	roa_float3 local_dir = roa_quaternion_rotate_vector(
		trans->rotation,
		roa_transform_world_up());

	return local_dir;
}


roa_float3
roa_transform_world_left()
{
	return roa_float3_set_with_values(+1.f, 0.f, 0.f);
}


roa_float3
roa_transform_local_left(const roa_transform *trans)
{
	roa_float3 local_dir = roa_quaternion_rotate_vector(
		trans->rotation,
		roa_transform_world_left());

	return local_dir;
}


/* -------------------------------------------------------------- [ mat2 ] -- */


void
roa_mat2_zero(roa_mat2 *out)
{
	assert(out);
	memset((void*)out->data, 0, sizeof(out->data));
}


void
roa_mat2_id(roa_mat2 *out)
{
	assert(out);

	roa_mat2_zero(out);

	out->data[0] = 1.f;
	out->data[3] = 1.f;
}


void
roa_mat2_fill(roa_mat2 *out, float value)
{
	assert(out);

	out->data[0] = value;
	out->data[1] = value;
	out->data[2] = value;
	out->data[3] = value;
}


void
roa_mat2_import(roa_mat2 *out, const float *in)
{
	assert(out);
	assert(in);

	out->data[0] = in[0];
	out->data[1] = in[1];
	out->data[2] = in[2];
	out->data[3] = in[3];
}


void
roa_mat2_multiply(roa_mat2 *out, const roa_mat2 *lhs, const roa_mat2 *rhs)
{
	assert(out);
	assert(lhs);
	assert(rhs);

	int i;
  for(i = 0; i < 4; ++i)
  {
    int row = (i / 2) * 2;
    int col = i % 2;

    roa_float2 left_vec = roa_float2_set_with_values(
      lhs->data[row + 0],
      lhs->data[row + 1]
    );

    roa_float2 right_vec = roa_float2_set_with_values(
      rhs->data[col + 0],
      rhs->data[col + 2]
    );

    float dot = roa_float2_dot(left_vec, right_vec);
    out->data[i] = dot;
  }
}


roa_float2
roa_mat2_multiply_with_float2(
  roa_float2 lhs,
  const roa_mat2 *rhs)
{
  float vec_data[2];
  int i;

  for (i = 0; i < 2; ++i)
  {
    const roa_float2 dot_vec = roa_float2_set_with_values(
      rhs->data[i + 0],
      rhs->data[i + 2]
    );

    vec_data[i] = roa_float2_dot(lhs, dot_vec);
  }

  return roa_float2_import(vec_data);
}


int
roa_mat2_is_near(const roa_mat2 *a, const roa_mat2 *b, float err)
{
	int count = 0;

	int i;
	for(i = 0; i < 4; ++i)
	{
		count += roa_float_is_near(a->data[i], b->data[i], err);
	}

	return count == 4 ? 1 : 0;
}


/* -------------------------------------------------------------- [ mat3 ] -- */


void
roa_mat3_zero(
	roa_mat3 *out)
{
	assert(out);

	memset((void*)out->data, 0, sizeof(out->data));
}


void
roa_mat3_id(
	roa_mat3 *out)
{
	assert(out);

	roa_mat3_zero(out);

	out->data[0] = 1.f;
	out->data[4] = 1.f;
	out->data[8] = 1.f;
}


void
roa_mat3_fill(
	roa_mat3 *out,
	float val)
{
	int i;

	for(i = 0; i < 9; ++i)
	{
		out->data[i] = val;
	}
}


void
roa_mat3_import(
	roa_mat3 *out,
	const float *in)
{
	assert(out);
	assert(in);

	memcpy(out->data, in, sizeof(out->data));
}


void
roa_mat3_add(
	roa_mat3 *out,
	const roa_mat3 *lhs,
	const roa_mat3 *rhs)
{
	int i;
	for(i = 0; i < 9; ++i)
	{
		out->data[i] = lhs->data[i] + rhs->data[i];
	}
}


void
roa_mat3_subtract(
	roa_mat3 *out,
	const roa_mat3 *lhs,
	const roa_mat3 *rhs)
{
	int i;
	for(i = 0; i < 9; ++i)
	{
		out->data[i] = lhs->data[i] - rhs->data[i];
	}
}


void
roa_mat3_multiply(
	roa_mat3 *out,
	const roa_mat3 *lhs,
	const roa_mat3 *rhs)
{
	int i;
  for(i = 0; i < 9; ++i)
  {
    /* [0,1,2,3] x [0,4,8,12] */
    int row = (i / 3) * 3;
    int col = i % 3;

    roa_float3 left_vec = roa_float3_set_with_values(
      lhs->data[row + 0],
      lhs->data[row + 1],
      lhs->data[row + 2]
    );

    roa_float3 right_vec = roa_float3_set_with_values(
      rhs->data[col + 0],
      rhs->data[col + 3],
      rhs->data[col + 6]
    );

    float dot = roa_float3_dot(left_vec, right_vec);
    out->data[i] = dot;
  }
}


roa_float3
roa_mat3_multiply_with_float3(
	roa_float3 lhs,
	const roa_mat3 *rhs)
{
  float vec_data[3];
	int i;

  for(i = 0; i < 3; ++i)
  {
    const roa_float3 dot_vec = roa_float3_set_with_values(
      rhs->data[i + 0],
      rhs->data[i + 3],
      rhs->data[i + 6]
    );

    vec_data[i] = roa_float3_dot(lhs, dot_vec);
  }

  return roa_float3_import(vec_data);
}


int
roa_mat3_is_near(
	const roa_mat3 *a,
	const roa_mat3 *b,
	float err)
{
	int count = 0;

	int i;
	for(i = 0; i < 9; ++i)
	{
		count += roa_float_is_near(a->data[i], b->data[i], err);
	}

	return count == 9 ? 1 : 0;
}


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
roa_mat4_fill(roa_mat4 *out, float value)
{
  int i;

  for (i = 0; i < 16; ++i)
  {
    out->data[i] = value;
  }
}


void
roa_mat4_import(roa_mat4 *out, const float *data)
{
  memcpy(out->data, data, sizeof(out->data));
}


void
roa_mat4_export(roa_mat4 *in, float *out)
{
  memcpy(out, in->data, sizeof(in->data));
}


void
roa_mat4_scale(roa_mat4 *out, roa_float3 scale)
{
  roa_mat4_id(out);

  out->data[0] = scale.x;
  out->data[5] = scale.y;
  out->data[10] = scale.z;
}


void
roa_mat4_translate(roa_mat4 *out, roa_float3 translate)
{
  roa_mat4_id(out);

  out->data[12] = translate.x;
  out->data[13] = translate.y;
  out->data[14] = translate.z;
}


void
roa_mat4_rotate(roa_mat4 *out, roa_quaternion rotation)
{
  roa_mat4_id(out);

  float x_sq = rotation.x * rotation.x;
  float y_sq = rotation.y * rotation.y;
  float z_sq = rotation.z * rotation.z;

  out->data[0] = 1 - 2 * y_sq - 2 * z_sq;
  out->data[1] = 2 * (rotation.x * rotation.y) + 2 * (rotation.z * rotation.w);
  out->data[2] = 2 * (rotation.x * rotation.z) - 2 * (rotation.y * rotation.w);

  out->data[4] = 2 * (rotation.x * rotation.y) - 2 * (rotation.z * rotation.w);
  out->data[5] = 1 - 2 * x_sq - 2 * z_sq;
  out->data[6] = 2 * (rotation.y * rotation.z) + 2 * (rotation.x * rotation.w);

  out->data[8] = 2 * (rotation.x * rotation.z) + 2 * (rotation.y * rotation.w);
  out->data[9] = 2 * (rotation.y * rotation.z) - 2 * (rotation.x * rotation.w);
  out->data[10] = 1 - 2 * x_sq - 2 * y_sq;
}


void
roa_mat4_inverse(roa_mat4 *out, const roa_mat4 *to_inverse)
{
  float temp[16];

    temp[0] =
    to_inverse->data[5]  * to_inverse->data[10] * to_inverse->data[15] -
    to_inverse->data[5]  * to_inverse->data[11] * to_inverse->data[14] -
    to_inverse->data[9]  * to_inverse->data[6]  * to_inverse->data[15] +
    to_inverse->data[9]  * to_inverse->data[7]  * to_inverse->data[14] +
    to_inverse->data[13] * to_inverse->data[6]  * to_inverse->data[11] -
    to_inverse->data[13] * to_inverse->data[7]  * to_inverse->data[10];

    temp[1] =
    -to_inverse->data[1]  * to_inverse->data[10] * to_inverse->data[15] +
    to_inverse->data[1]  * to_inverse->data[11] * to_inverse->data[14] +
    to_inverse->data[9]  * to_inverse->data[2] * to_inverse->data[15] -
    to_inverse->data[9]  * to_inverse->data[3] * to_inverse->data[14] -
    to_inverse->data[13] * to_inverse->data[2] * to_inverse->data[11] +
    to_inverse->data[13] * to_inverse->data[3] * to_inverse->data[10];

    temp[2] =
    to_inverse->data[1]  * to_inverse->data[6] * to_inverse->data[15] -
    to_inverse->data[1]  * to_inverse->data[7] * to_inverse->data[14] -
    to_inverse->data[5]  * to_inverse->data[2] * to_inverse->data[15] +
    to_inverse->data[5]  * to_inverse->data[3] * to_inverse->data[14] +
    to_inverse->data[13] * to_inverse->data[2] * to_inverse->data[7] -
    to_inverse->data[13] * to_inverse->data[3] * to_inverse->data[6];

    temp[3] =
    -to_inverse->data[1] * to_inverse->data[6] * to_inverse->data[11] +
    to_inverse->data[1] * to_inverse->data[7] * to_inverse->data[10] +
    to_inverse->data[5] * to_inverse->data[2] * to_inverse->data[11] -
    to_inverse->data[5] * to_inverse->data[3] * to_inverse->data[10] -
    to_inverse->data[9] * to_inverse->data[2] * to_inverse->data[7] +
    to_inverse->data[9] * to_inverse->data[3] * to_inverse->data[6];

    temp[4] =
    -to_inverse->data[4]  * to_inverse->data[10] * to_inverse->data[15] +
    to_inverse->data[4]  * to_inverse->data[11] * to_inverse->data[14] +
    to_inverse->data[8]  * to_inverse->data[6]  * to_inverse->data[15] -
    to_inverse->data[8]  * to_inverse->data[7]  * to_inverse->data[14] -
    to_inverse->data[12] * to_inverse->data[6]  * to_inverse->data[11] +
    to_inverse->data[12] * to_inverse->data[7]  * to_inverse->data[10];

    temp[5] =
    to_inverse->data[0]  * to_inverse->data[10] * to_inverse->data[15] -
    to_inverse->data[0]  * to_inverse->data[11] * to_inverse->data[14] -
    to_inverse->data[8]  * to_inverse->data[2] * to_inverse->data[15] +
    to_inverse->data[8]  * to_inverse->data[3] * to_inverse->data[14] +
    to_inverse->data[12] * to_inverse->data[2] * to_inverse->data[11] -
    to_inverse->data[12] * to_inverse->data[3] * to_inverse->data[10];

    temp[6] =
    -to_inverse->data[0]  * to_inverse->data[6] * to_inverse->data[15] +
    to_inverse->data[0]  * to_inverse->data[7] * to_inverse->data[14] +
    to_inverse->data[4]  * to_inverse->data[2] * to_inverse->data[15] -
    to_inverse->data[4]  * to_inverse->data[3] * to_inverse->data[14] -
    to_inverse->data[12] * to_inverse->data[2] * to_inverse->data[7] +
    to_inverse->data[12] * to_inverse->data[3] * to_inverse->data[6];

    temp[7] =
    to_inverse->data[0] * to_inverse->data[6] * to_inverse->data[11] -
    to_inverse->data[0] * to_inverse->data[7] * to_inverse->data[10] -
    to_inverse->data[4] * to_inverse->data[2] * to_inverse->data[11] +
    to_inverse->data[4] * to_inverse->data[3] * to_inverse->data[10] +
    to_inverse->data[8] * to_inverse->data[2] * to_inverse->data[7] -
    to_inverse->data[8] * to_inverse->data[3] * to_inverse->data[6];

    temp[8] =
    to_inverse->data[4]  * to_inverse->data[9] * to_inverse->data[15] -
    to_inverse->data[4]  * to_inverse->data[11] * to_inverse->data[13] -
    to_inverse->data[8]  * to_inverse->data[5] * to_inverse->data[15] +
    to_inverse->data[8]  * to_inverse->data[7] * to_inverse->data[13] +
    to_inverse->data[12] * to_inverse->data[5] * to_inverse->data[11] -
    to_inverse->data[12] * to_inverse->data[7] * to_inverse->data[9];

    temp[9] =
    -to_inverse->data[0]  * to_inverse->data[9] * to_inverse->data[15] +
    to_inverse->data[0]  * to_inverse->data[11] * to_inverse->data[13] +
    to_inverse->data[8]  * to_inverse->data[1] * to_inverse->data[15] -
    to_inverse->data[8]  * to_inverse->data[3] * to_inverse->data[13] -
    to_inverse->data[12] * to_inverse->data[1] * to_inverse->data[11] +
    to_inverse->data[12] * to_inverse->data[3] * to_inverse->data[9];

    temp[10] =
    to_inverse->data[0]  * to_inverse->data[5] * to_inverse->data[15] -
    to_inverse->data[0]  * to_inverse->data[7] * to_inverse->data[13] -
    to_inverse->data[4]  * to_inverse->data[1] * to_inverse->data[15] +
    to_inverse->data[4]  * to_inverse->data[3] * to_inverse->data[13] +
    to_inverse->data[12] * to_inverse->data[1] * to_inverse->data[7] -
    to_inverse->data[12] * to_inverse->data[3] * to_inverse->data[5];

    temp[11] =
    -to_inverse->data[0] * to_inverse->data[5] * to_inverse->data[11] +
    to_inverse->data[0] * to_inverse->data[7] * to_inverse->data[9] +
    to_inverse->data[4] * to_inverse->data[1] * to_inverse->data[11] -
    to_inverse->data[4] * to_inverse->data[3] * to_inverse->data[9] -
    to_inverse->data[8] * to_inverse->data[1] * to_inverse->data[7] +
    to_inverse->data[8] * to_inverse->data[3] * to_inverse->data[5];

    temp[12] =
    -to_inverse->data[4]  * to_inverse->data[9] * to_inverse->data[14] +
    to_inverse->data[4]  * to_inverse->data[10] * to_inverse->data[13] +
    to_inverse->data[8]  * to_inverse->data[5] * to_inverse->data[14] -
    to_inverse->data[8]  * to_inverse->data[6] * to_inverse->data[13] -
    to_inverse->data[12] * to_inverse->data[5] * to_inverse->data[10] +
    to_inverse->data[12] * to_inverse->data[6] * to_inverse->data[9];

    temp[13] =
    to_inverse->data[0]  * to_inverse->data[9] * to_inverse->data[14] -
    to_inverse->data[0]  * to_inverse->data[10] * to_inverse->data[13] -
    to_inverse->data[8]  * to_inverse->data[1] * to_inverse->data[14] +
    to_inverse->data[8]  * to_inverse->data[2] * to_inverse->data[13] +
    to_inverse->data[12] * to_inverse->data[1] * to_inverse->data[10] -
    to_inverse->data[12] * to_inverse->data[2] * to_inverse->data[9];

    temp[14] =
    -to_inverse->data[0]  * to_inverse->data[5] * to_inverse->data[14] +
    to_inverse->data[0]  * to_inverse->data[6] * to_inverse->data[13] +
    to_inverse->data[4]  * to_inverse->data[1] * to_inverse->data[14] -
    to_inverse->data[4]  * to_inverse->data[2] * to_inverse->data[13] -
    to_inverse->data[12] * to_inverse->data[1] * to_inverse->data[6] +
    to_inverse->data[12] * to_inverse->data[2] * to_inverse->data[5];

    temp[15] =
    to_inverse->data[0] * to_inverse->data[5] * to_inverse->data[10] -
    to_inverse->data[0] * to_inverse->data[6] * to_inverse->data[9] -
    to_inverse->data[4] * to_inverse->data[1] * to_inverse->data[10] +
    to_inverse->data[4] * to_inverse->data[2] * to_inverse->data[9] +
    to_inverse->data[8] * to_inverse->data[1] * to_inverse->data[6] -
    to_inverse->data[8] * to_inverse->data[2] * to_inverse->data[5];

  const float determinant = roa_mat4_determinant(to_inverse);

  assert(determinant != 0);

  const float one_over_det = 1.f / determinant;

  float inverse[16];
  memset(inverse, 0, sizeof(inverse));

  for(size_t i = 0; i < 16; ++i)
  {
    inverse[i] = temp[i] * one_over_det;
  }

  roa_mat4_import(out, inverse);
}


float
roa_mat4_determinant(const roa_mat4 *det)
{
  float temp[16];

  temp[0] =
   det->data[5]  *det->data[10] *det->data[15] -
   det->data[5]  *det->data[11] *det->data[14] -
   det->data[9]  *det->data[6]  *det->data[15] +
   det->data[9]  *det->data[7]  *det->data[14] +
   det->data[13] *det->data[6]  *det->data[11] -
   det->data[13] *det->data[7]  *det->data[10];

   temp[1] =
   -det->data[1]  *det->data[10] *det->data[15] +
   det->data[1]  *det->data[11] *det->data[14] +
   det->data[9]  *det->data[2] *det->data[15] -
   det->data[9]  *det->data[3] *det->data[14] -
   det->data[13] *det->data[2] *det->data[11] +
   det->data[13] *det->data[3] *det->data[10];

   temp[2] =
   det->data[1]  *det->data[6] *det->data[15] -
   det->data[1]  *det->data[7] *det->data[14] -
   det->data[5]  *det->data[2] *det->data[15] +
   det->data[5]  *det->data[3] *det->data[14] +
   det->data[13] *det->data[2] *det->data[7] -
   det->data[13] *det->data[3] *det->data[6];

   temp[3] =
   -det->data[1] *det->data[6] *det->data[11] +
   det->data[1] *det->data[7] *det->data[10] +
   det->data[5] *det->data[2] *det->data[11] -
   det->data[5] *det->data[3] *det->data[10] -
   det->data[9] *det->data[2] *det->data[7] +
   det->data[9] *det->data[3] *det->data[6];

   temp[4] =
    -det->data[4]  *det->data[10] *det->data[15] +
   det->data[4]  *det->data[11] *det->data[14] +
   det->data[8]  *det->data[6]  *det->data[15] -
   det->data[8]  *det->data[7]  *det->data[14] -
   det->data[12] *det->data[6]  *det->data[11] +
   det->data[12] *det->data[7]  *det->data[10];

   temp[5] =
   det->data[0]  *det->data[10] *det->data[15] -
   det->data[0]  *det->data[11] *det->data[14] -
   det->data[8]  *det->data[2] *det->data[15] +
   det->data[8]  *det->data[3] *det->data[14] +
   det->data[12] *det->data[2] *det->data[11] -
   det->data[12] *det->data[3] *det->data[10];

   temp[6] =
   -det->data[0]  *det->data[6] *det->data[15] +
   det->data[0]  *det->data[7] *det->data[14] +
   det->data[4]  *det->data[2] *det->data[15] -
   det->data[4]  *det->data[3] *det->data[14] -
   det->data[12] *det->data[2] *det->data[7] +
   det->data[12] *det->data[3] *det->data[6];

   temp[7] =
   det->data[0] *det->data[6] *det->data[11] -
   det->data[0] *det->data[7] *det->data[10] -
   det->data[4] *det->data[2] *det->data[11] +
   det->data[4] *det->data[3] *det->data[10] +
   det->data[8] *det->data[2] *det->data[7] -
   det->data[8] *det->data[3] *det->data[6];

   temp[8] =
   det->data[4]  *det->data[9] *det->data[15] -
   det->data[4]  *det->data[11] *det->data[13] -
   det->data[8]  *det->data[5] *det->data[15] +
   det->data[8]  *det->data[7] *det->data[13] +
   det->data[12] *det->data[5] *det->data[11] -
   det->data[12] *det->data[7] *det->data[9];

   temp[9] =
   -det->data[0]  *det->data[9] *det->data[15] +
   det->data[0]  *det->data[11] *det->data[13] +
   det->data[8]  *det->data[1] *det->data[15] -
   det->data[8]  *det->data[3] *det->data[13] -
   det->data[12] *det->data[1] *det->data[11] +
   det->data[12] *det->data[3] *det->data[9];

   temp[10] =
   det->data[0]  *det->data[5] *det->data[15] -
   det->data[0]  *det->data[7] *det->data[13] -
   det->data[4]  *det->data[1] *det->data[15] +
   det->data[4]  *det->data[3] *det->data[13] +
   det->data[12] *det->data[1] *det->data[7] -
   det->data[12] *det->data[3] *det->data[5];

   temp[11] =
   -det->data[0] *det->data[5] *det->data[11] +
   det->data[0] *det->data[7] *det->data[9] +
   det->data[4] *det->data[1] *det->data[11] -
   det->data[4] *det->data[3] *det->data[9] -
   det->data[8] *det->data[1] *det->data[7] +
   det->data[8] *det->data[3] *det->data[5];

    temp[12] =
    -det->data[4]  *det->data[9] *det->data[14] +
   det->data[4]  *det->data[10] *det->data[13] +
   det->data[8]  *det->data[5] *det->data[14] -
   det->data[8]  *det->data[6] *det->data[13] -
   det->data[12] *det->data[5] *det->data[10] +
   det->data[12] *det->data[6] *det->data[9];

    temp[13] =
   det->data[0]  *det->data[9] *det->data[14] -
   det->data[0]  *det->data[10] *det->data[13] -
   det->data[8]  *det->data[1] *det->data[14] +
   det->data[8]  *det->data[2] *det->data[13] +
   det->data[12] *det->data[1] *det->data[10] -
   det->data[12] *det->data[2] *det->data[9];

    temp[14] =
    -det->data[0]  *det->data[5] *det->data[14] +
   det->data[0]  *det->data[6] *det->data[13] +
   det->data[4]  *det->data[1] *det->data[14] -
   det->data[4]  *det->data[2] *det->data[13] -
   det->data[12] *det->data[1] *det->data[6] +
   det->data[12] *det->data[2] *det->data[5];

    temp[15] =
   det->data[0] *det->data[5] *det->data[10] -
   det->data[0] *det->data[6] *det->data[9] -
   det->data[4] *det->data[1] *det->data[10] +
   det->data[4] *det->data[2] *det->data[9] +
   det->data[8] *det->data[1] *det->data[6] -
   det->data[8] *det->data[2] *det->data[5];

  float determinant = det->data[0] * temp[0] +
                      det->data[1] * temp[4] +
                      det->data[2] * temp[8] +
                      det->data[3] * temp[12];

  return determinant;
}


void
roa_mat4_fill_with_value(roa_mat4 *out, float val)
{
  assert(out);

	int i;
  for (i = 0; i < 16; ++i)
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
roa_mat4_multiply(roa_mat4 *out, const roa_mat4 *lhs, const roa_mat4 *rhs)
{
  assert(out);
  assert(lhs);
  assert(rhs);

	int i;
  for (i = 0; i < 16; ++i)
  {
    /* Starting index for data. */
    unsigned row = (i / 4) * 4;
    unsigned col = (i % 4);

    roa_float4 row_vec = roa_float4_set_with_values(
      lhs->data[row + 0],
      lhs->data[row + 1],
      lhs->data[row + 2],
      lhs->data[row + 3]
    );

    roa_float4 col_vec = roa_float4_set_with_values(
      rhs->data[col + 0],
      rhs->data[col + 4],
      rhs->data[col + 8],
      rhs->data[col + 12]
    );

    float dot = roa_float4_dot(row_vec, col_vec);

    out->data[i] = dot;
  }
}


void
roa_mat4_multiply_three(
	roa_mat4 *out,
	const roa_mat4 *a,
	const roa_mat4 *b,
	const roa_mat4 *c)
{
	roa_mat4 temp;
  roa_mat4_multiply(&temp, a, b);
	roa_mat4_multiply(out, &temp, c);
}


int
roa_mat4_is_near(
  const roa_mat4 *a,
  const roa_mat4 *b,
  float err)
{
  int count = 0;

  int i;
  for (i = 0; i < 16; ++i)
  {
    count += roa_float_is_near(a->data[i], b->data[i], err);
  }

  return count == 16 ? 1 : 0;
}


/* -------------------------------------------------------------- [ ray ] -- */
/* ------------------------------------------------------------- [ aabb ] -- */

/* ---------------------------------------------------------- [ helpers ] -- */


#undef MATH_ARR_COUNT
