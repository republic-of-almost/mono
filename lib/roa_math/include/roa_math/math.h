#ifndef MATH_INCLUDED_9EC54CED_2CD3_493C_92F2_88DC9666169B
#define MATH_INCLUDED_9EC54CED_2CD3_493C_92F2_88DC9666169B


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------ [ types ] -- */


typedef struct roa_float2 { float x, y; }                     roa_float2;
typedef struct roa_float3 { float x, y, z; }                  roa_float3;
typedef struct roa_float4 { float x, y, z, w; }               roa_float4;
typedef struct roa_quaternion { float x, y, z, w; }           roa_quaternion;
typedef struct roa_transform { roa_float3 position; roa_float3 scale; roa_quaternion rotation; } roa_transform;
typedef struct roa_mat3 { float data[9];  }                   roa_mat3;
typedef struct roa_mat4 { float data[16]; }                   roa_mat4;
typedef struct roa_aabb { roa_float3 min; roa_float3 max; }   roa_aabb;
typedef struct roa_ray { roa_float3 origin; roa_float3 dir; } roa_ray;


/* -------------------------------------------------------- [ constants ] -- */


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


/* ---------------------------------------------------------- [ general ] -- */


float           roa_float_tan(float a);
float           roa_float_sin(float a);
float           roa_float_cos(float a);

float           roa_float_sqrt(float a);

float           roa_float_abs(float a);
int             roa_float_is_near(float a, float b, float err);


/* ----------------------------------------------------------- [ float2 ] -- */
/* ----------------------------------------------------------- [ float3 ] -- */


roa_float3      roa_float3_zero();
roa_float3      roa_float3_zero_zero_one();
roa_float3      roa_float3_one();
roa_float3      roa_float3_fill_with_value(float v);
roa_float3      roa_float3_set_with_values(float x, float y, float z);

void            roa_float3_export(roa_float3 a, float *out);

float           roa_float3_get_x(roa_float3 a);
float           roa_float3_get_y(roa_float3 a);
float           roa_float3_get_z(roa_float3 a);

roa_float3      roa_float3_add(roa_float3 a, roa_float3 b);
roa_float3      roa_float3_subtract(roa_float3 a, roa_float3 b);
roa_float3      roa_float3_multiply(roa_float3 a, roa_float3 b);

float           roa_float3_length(roa_float3 a);
roa_float3      roa_float3_normalize(roa_float3 a);
float           roa_float3_dot(roa_float3 a, roa_float3 b);
roa_float3      roa_float3_cross(roa_float3 a, roa_float3 b);
roa_float3      roa_float3_scale(roa_float3 a, float scale);

int             roa_float3_is_near(roa_float3 a, roa_float3 b, float err);


/* ----------------------------------------------------------- [ float4 ] -- */


roa_float4      roa_float4_zero();
roa_float4      roa_float4_zero_zero_zero_one();
roa_float4      roa_float4_one();
roa_float4      roa_float4_fill_with_value(float v);
roa_float4      roa_float4_set_with_values(float x, float y, float z, float w);

void            roa_float4_export(roa_float4 a, float *out);

float           roa_float4_get_x(roa_float4 a);
float           roa_float4_get_y(roa_float4 a);
float           roa_float4_get_z(roa_float4 a);
float           roa_float4_get_w(roa_float4 a);

roa_float4      roa_float4_add(roa_float4 a, roa_float4 b);
roa_float4      roa_float4_subtract(roa_float4 a, roa_float4 b);
roa_float4      roa_float4_multiply(roa_float4 a, roa_float4 b);

float           roa_float4_length(roa_float4 a);
roa_float4      roa_float4_normalize(roa_float4 a);
float           roa_float4_dot(roa_float4 a, roa_float4 b);
roa_float4      roa_float4_scale(roa_float4 a, float scale);

int             roa_float4_is_near(roa_float4 a, roa_float4 b, float err);


/* ------------------------------------------------------- [ quaternion ] -- */


roa_quaternion  roa_quaternion_default();
roa_quaternion  roa_quaternion_set_with_values(float x, float y, float z, float w);
roa_quaternion  roa_quaternion_multiply(roa_quaternion left, roa_quaternion right);

int             roa_quaternion_is_near(roa_quaternion a, roa_quaternion b, float err);


/* -------------------------------------------------------- [ transform ] -- */


void            roa_transform_init(roa_transform *out);
void            roa_transform_inherited(roa_transform *out, const roa_transform *parent, const roa_transform *local);

void            roa_transform_to_mat4(const roa_transform *trans, roa_mat4 *out);
void            roa_transform_to_lookat_vectors(const roa_transform *trans, roa_float3 *out_pos, roa_float3 *out_dir, roa_float3 *out_up);


/* ------------------------------------------------------------- [ mat3 ] -- */
/* ------------------------------------------------------------- [ mat4 ] -- */


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

void            roa_mat4_multiply(roa_mat4 *out, roa_mat4 *a, roa_mat4 *b);
void            roa_mat4_multiply_three(roa_mat4 *out, roa_mat4 *a, roa_mat4 *b, roa_mat4 *c);


/* -------------------------------------------------------------- [ ray ] -- */
/* ------------------------------------------------------------- [ aabb ] -- */


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
