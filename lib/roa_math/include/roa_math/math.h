#ifndef MATH_INCLUDED_9EC54CED_2CD3_493C_92F2_88DC9666169B
#define MATH_INCLUDED_9EC54CED_2CD3_493C_92F2_88DC9666169B


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------- [ types ] -- */


typedef struct roa_float2 { float x, y; }                     roa_float2;
typedef struct roa_float3 { float x, y, z; }                  roa_float3;
typedef struct roa_float4 { float x, y, z, w; }               roa_float4;
typedef struct roa_quaternion { float x, y, z, w; }           roa_quaternion;
typedef struct roa_transform { roa_float3 position; roa_float3 scale; roa_quaternion rotation; } roa_transform;
typedef struct roa_mat2 { float data[4];  }                   roa_mat2;
typedef struct roa_mat3 { float data[9];  }                   roa_mat3;
typedef struct roa_mat4 { float data[16]; }                   roa_mat4;
typedef struct roa_aabb { roa_float3 min; roa_float3 max; }   roa_aabb;
typedef struct roa_ray { roa_float3 origin; roa_float3 dir; } roa_ray;


/* --------------------------------------------------------- [ constants ] -- */


#define         ROA_PI 3.14159265359
#define         ROA_TWO_PI 6.28318530718
#define         ROA_HALF_PI 1.5707963268
#define         ROA_TAU 6.28318530718
#define         ROA_HALF_TAU 3.14159265359
#define         ROA_QUART_TAU 1.5707963268
#define         ROA_G_RATIO 1.61803398875
#define         ROA_ROOT_TWO 1.41421356237
#define         ROA_ROOT_THREE 1.73205080757
#define         ROA_EPSILON 0.000000001


/* ----------------------------------------------------------- [ general ] -- */


float           roa_float_tan(float a);
float           roa_float_sin(float a);
float           roa_float_cos(float a);

float           roa_float_sqrt(float a);

float           roa_float_sign(float a);
float           roa_float_fract(float a);
float           roa_float_round(float a);
float           roa_float_floor(float a);
float           roa_float_ceil(float a);
float           roa_float_lerp(float a, float b, float mix);
float           roa_float_max(float a, float b);
float           roa_float_min(float a, float b);
float           roa_float_clamp(float val, float a, float b);

float           roa_float_abs(float a);
int             roa_float_is_near(float a, float b, float err);


/* ------------------------------------------------------------ [ float2 ] -- */


roa_float2      roa_float2_zero();
roa_float2      roa_float2_zero_one();
roa_float2      roa_float2_one();
roa_float2      roa_float2_fill_with_value(float v);
roa_float2      roa_float2_set_with_values(float x, float y);
roa_float2      roa_float2_import(const float *in);
void            roa_float2_export(roa_float2 a, float *out);

roa_float2      roa_float2_add(roa_float2 a, roa_float2 b);
roa_float2      roa_float2_subtract(roa_float2 a, roa_float2 b);
roa_float2      roa_float2_multiply(roa_float2 a, roa_float2 b);
roa_float2      roa_float2_divide(roa_float2 a, roa_float2 b);
roa_float2      roa_float2_scale(roa_float2 a, float scale);
roa_float2      roa_float2_floor(roa_float2 a);
roa_float2      roa_float2_fract(roa_float2 a);
float           roa_float2_squared_length(roa_float2 a);
float           roa_float2_length(roa_float2 a);

float           roa_float2_get_x(roa_float2 a);
float           roa_float2_get_y(roa_float2 a);

float           roa_float2_dot(roa_float2 a, roa_float2 b);


/* ------------------------------------------------------------ [ float3 ] -- */


roa_float3      roa_float3_zero();
roa_float3      roa_float3_zero_zero_one();
roa_float3      roa_float3_one();
roa_float3      roa_float3_fill_with_value(float v);
roa_float3      roa_float3_set_with_values(float x, float y, float z);

roa_float3      roa_float3_import(const float *in);
void            roa_float3_export(roa_float3 a, float *out);

float           roa_float3_get_x(roa_float3 a);
float           roa_float3_get_y(roa_float3 a);
float           roa_float3_get_z(roa_float3 a);

roa_float3      roa_float3_add(roa_float3 a, roa_float3 b);
roa_float3      roa_float3_subtract(roa_float3 a, roa_float3 b);
roa_float3      roa_float3_multiply(roa_float3 a, roa_float3 b);

roa_float3      roa_float3_lerp(roa_float3 a, roa_float3 b, float t);
roa_float3      roa_float3_reflect(roa_float3 v, roa_float3 n);
float           roa_float3_length(roa_float3 a);
float           roa_float3_squared_length(roa_float3 a);
roa_float3      roa_float3_normalize(roa_float3 a);
float           roa_float3_dot(roa_float3 a, roa_float3 b);
roa_float3      roa_float3_cross(roa_float3 a, roa_float3 b);
roa_float3      roa_float3_scale(roa_float3 a, float scale);

roa_float3      roa_float3_fract(roa_float3 a);
roa_float3      roa_float3_floor(roa_float3 a);
roa_float3      roa_float3_ceil(roa_float3 a);


int             roa_float3_is_near(roa_float3 a, roa_float3 b, float err);


/* ------------------------------------------------------------ [ float4 ] -- */


roa_float4      roa_float4_zero();
roa_float4      roa_float4_zero_zero_zero_one();
roa_float4      roa_float4_one();
roa_float4      roa_float4_fill_with_value(float v);
roa_float4      roa_float4_set_with_values(float x, float y, float z, float w);

roa_float4      roa_float4_import(const float *in);
void            roa_float4_export(roa_float4 a, float *out);

float           roa_float4_get_x(roa_float4 a);
float           roa_float4_get_y(roa_float4 a);
float           roa_float4_get_z(roa_float4 a);
float           roa_float4_get_w(roa_float4 a);

roa_float4      roa_float4_add(roa_float4 a, roa_float4 b);
roa_float4      roa_float4_subtract(roa_float4 a, roa_float4 b);
roa_float4      roa_float4_multiply(roa_float4 a, roa_float4 b);

roa_float4      roa_float4_lerp(roa_float4 a, roa_float4 b, float t);
float           roa_float4_length(roa_float4 a);
roa_float4      roa_float4_normalize(roa_float4 a);
float           roa_float4_dot(roa_float4 a, roa_float4 b);
roa_float4      roa_float4_scale(roa_float4 a, float scale);

int             roa_float4_is_near(roa_float4 a, roa_float4 b, float err);


/* -------------------------------------------------------- [ quaternion ] -- */


roa_quaternion  roa_quaternion_default();
roa_quaternion  roa_quaternion_set_with_values(float x, float y, float z, float w);
roa_quaternion  roa_quaternion_multiply(roa_quaternion left, roa_quaternion right);

int             roa_quaternion_is_near(roa_quaternion a, roa_quaternion b, float err);

void            roa_quaternion_get_rotation_matrix(roa_quaternion rotation, roa_mat3 *out);
roa_float3      roa_quaternion_rotate_vector(roa_quaternion rotation, roa_float3 vector);

float           roa_quaternion_get_x(roa_quaternion a);
float           roa_quaternion_get_y(roa_quaternion a);
float           roa_quaternion_get_z(roa_quaternion a);
float           roa_quaternion_get_w(roa_quaternion a);

/* --------------------------------------------------------- [ transform ] -- */


void            roa_transform_init(roa_transform *out);
void            roa_transform_inherited(roa_transform *out, const roa_transform *parent, const roa_transform *local);

void            roa_transform_to_mat4(const roa_transform *trans, roa_mat4 *out);

roa_float3      roa_transform_world_fwd();
roa_float3      roa_transform_local_fwd(const roa_transform *trans);

roa_float3      roa_transform_world_up();
roa_float3      roa_transform_local_up(const roa_transform *trans);

roa_float3      roa_transform_world_left();
roa_float3      roa_transform_local_left(const roa_transform *trans);


/* -------------------------------------------------------------- [ mat2 ] -- */


void            roa_mat2_zero(roa_mat2 *out);
void            roa_mat2_id(roa_mat2 *out);
void            roa_mat2_fill(roa_mat2 *out, float value);
void            roa_mat2_import(roa_mat2 *out, const float *in);

void            roa_mat2_multiply(roa_mat2 *out, const roa_mat2 *lhs, const roa_mat2 *rhs);
roa_float2      roa_mat2_multiply_with_float2(roa_float2 lhs, const roa_mat2 *rhs);

int             roa_mat2_is_near(const roa_mat2 *a, const roa_mat2 *b, float error);


/* -------------------------------------------------------------- [ mat3 ] -- */


void            roa_mat3_zero(roa_mat3 *out);
void            roa_mat3_id(roa_mat3 *out);
void            roa_mat3_fill(roa_mat3 *out, float value);
void            roa_mat3_import(roa_mat3 *out, const float *in);

void            roa_mat3_add(roa_mat3 *out, const roa_mat3 *left, const roa_mat3 *right);
void            roa_mat3_subtract(roa_mat3 *out, const roa_mat3 *left, const roa_mat3 *right);
void            roa_mat3_multiply(roa_mat3 *out, const roa_mat3 *left, const roa_mat3 *rhs);
roa_float3      roa_mat3_multiply_with_float3(roa_float3 lhs, const roa_mat3 *rhs);

int             roa_mat3_is_near(roa_mat3 *a, roa_mat3 *b, float error);


/* -------------------------------------------------------------- [ mat4 ] -- */


void            roa_mat4_zero(roa_mat4 *out);
void            roa_mat4_id(roa_mat4 *out);
void            roa_mat4_import(roa_mat4 *out, float *data);
void            roa_mat4_export(roa_mat4 *in, float *out);

void            roa_mat4_scale(roa_mat4 *out, roa_float3 scale);
void            roa_mat4_translate(roa_mat4 *out, roa_float3 translate);
void            roa_mat4_rotate(roa_mat4 *out, roa_quaternion rotation);

void            roa_mat4_fill_with_value(roa_mat4 *out, float val);

void            roa_mat4_projection(roa_mat4 *out, float fov, float near, float far, float aspect_ratio);
void            roa_mat4_lookat(roa_mat4 *out, roa_float3 from, roa_float3 to, roa_float3 up);

void            roa_mat4_multiply(roa_mat4 *out, const roa_mat4 *a, const roa_mat4 *b);
void            roa_mat4_multiply_three(roa_mat4 *out, const roa_mat4 *a, const roa_mat4 *b, const roa_mat4 *c);


/* --------------------------------------------------------------- [ ray ] -- */
/* -------------------------------------------------------------- [ aabb ] -- */


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */

