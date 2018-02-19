#ifndef MATH_INCLUDED_
#define MATH_INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------------- [ types ] -- */


typedef struct float2 { float data[2];  }         float2;
typedef struct float3 { float data[3];  }         float3;
typedef struct float4 { float data[4];  }         float4;
typedef struct mat3   { float data[9];  }         mat3;
typedef struct mat4   { float data[16]; }         mat4;
typedef struct aabb   { float3 min; float3 max; } aabb;


/* -- [ float2 ] -- */

float       float2_get_x(float2 a);
float       float2_get_y(float2 a);

float2      float2_add(float2 a, float2 b);
float2      float2_subtract(float2 a, float2 b);
float2      float2_multiply(float2 a, float2 b);

/* -- [ float3 ] -- */


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
