#ifndef TO_STRING_INCLUDED_AB8A4646_3D89_48D2_AE8B_908920B6D905
#define TO_STRING_INCLUDED_AB8A4646_3D89_48D2_AE8B_908920B6D905


#include "detail/detail.hpp"
#include "mat/mat4.hpp"
#include "mat/mat3.hpp"
#include "transform/transform.hpp"
#include "vec/vec3.hpp"
#include "vec/vec4.hpp"
#include "quat/quat.hpp"
#include <string.h>


// --------------------------------------------------------------- [ Config ] --


#define MATH_TO_STRING_INLINE inline


namespace MATH_NS_NAME {


// ------------------------------------------------------------ [ Interface ] --


MATH_TO_STRING_INLINE void    to_string(const mat4 &str, char out_buffer[64]);
MATH_TO_STRING_INLINE void    to_string(const mat3 &str, char out_buffer[64]);
MATH_TO_STRING_INLINE void    to_string(const vec3 &str, char out_buffer[32]);
MATH_TO_STRING_INLINE void    to_string(const vec4 &str, char out_buffer[32]);
MATH_TO_STRING_INLINE void    to_string(const quat &str, char out_buffer[32]);
MATH_TO_STRING_INLINE void    to_string(const transform &str, char out_buffer[128]);


// ----------------------------------------------------------------- [ Impl ] --


void
to_string(const mat4 &mat, char out_buffer[64])
{
  memset(out_buffer, 0, sizeof(char) * 64);
  
  sprintf(
    out_buffer,
    "%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f\n%.2f %.2f %.2f %.2f",
    mat.data[0],
    mat.data[1],
    mat.data[2],
    mat.data[3],
    mat.data[4],
    mat.data[5],
    mat.data[6],
    mat.data[7],
    mat.data[8],
    mat.data[9],
    mat.data[10],
    mat.data[11],
    mat.data[12],
    mat.data[13],
    mat.data[14],
    mat.data[15]
  );
}


void
to_string(const mat3 &mat, char out_buffer[64])
{
  memset(out_buffer, 0, sizeof(char) * 64);
  
  sprintf(
    out_buffer,
    "%.2f %.2f %.2f\n%.2f %.2f %.2f\n%.2f %.2f %.2f",
    mat.data[0],
    mat.data[1],
    mat.data[2],
    mat.data[3],
    mat.data[4],
    mat.data[5],
    mat.data[6],
    mat.data[7],
    mat.data[8]
  );
}


void
to_string(const vec3 &str, char out_buffer[32])
{
  memset(out_buffer, 0, sizeof(char) * 32);
  
  sprintf(out_buffer, "%.2f %.2f %.2f", str.data[0], str.data[1], str.data[2]);
}


void
to_string(const vec4 &str, char out_buffer[32])
{
  memset(out_buffer, 0, sizeof(char) * 32);
  
  sprintf(
    out_buffer,
    "%.2f %.2f %.2f %.2f",
    str.data[0],
    str.data[1],
    str.data[2],
    str.data[3]
  );
}


void
to_string(const quat &str, char out_buffer[32])
{
  memset(out_buffer, 0, sizeof(char) * 32);
  
  sprintf(
    out_buffer,
    "%.2f %.2f %.2f %.2f",
    str.data[0],
    str.data[1],
    str.data[2],
    str.data[3]
  );
}



void
to_string(const transform &str, char out_buffer[128])
{
  memset(out_buffer, 0, sizeof(char) * 128);
  
  strcat(out_buffer, "Pos: ");
  to_string(str.position, &out_buffer[strlen(out_buffer)]);
  strcat(out_buffer, "\n");
  
  strcat(out_buffer, "Scale: ");
  to_string(str.scale, &out_buffer[strlen(out_buffer)]);
  strcat(out_buffer, "\n");
  
  strcat(out_buffer, "Rot: ");
  to_string(str.rotation, &out_buffer[strlen(out_buffer)]);
  strcat(out_buffer, "\n");
}


} // ns


#endif // inc guard
