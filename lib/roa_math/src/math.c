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
roa_float_sqrt(float a)
{
  return sqrtf(a);
}


float
roa_float_fract(float a)
{
  return a - (long)a;
}


float
roa_float_round(float a)
{
  return (int)(a + 0.5f);
}


float
roa_float_floor(float a)
{
  return roa_float_round(a - 1);
}


float
roa_float_ceil(float a)
{
  return roa_float_round(a + 1);
}


float
roa_float_lerp(float a, float b, float mix)
{
  return a + mix * (b - a);
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


/* ------------------------------------------------------------- [ mat3 ] -- */


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
	roa_mat3 *a,
	roa_mat3 *b,
	float err)
{
	int count = 0;

	int i;
	for(i = 0; i < 9; ++i)
	{
		count += roa_float_is_near(a->data[i], b->data[i], err);
	}

	return count == 9;
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
roa_mat4_import(roa_mat4 *out, float *data)
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


/* -------------------------------------------------------------- [ ray ] -- */
/* ------------------------------------------------------------- [ aabb ] -- */

/* ---------------------------------------------------------- [ helpers ] -- */


#undef MATH_ARR_COUNT
