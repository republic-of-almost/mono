#ifndef GENERAL_INCLUDED_BA1581F2_35C3_4026_BCDA_6E9EC3CF253E
#define GENERAL_INCLUDED_BA1581F2_35C3_4026_BCDA_6E9EC3CF253E


/*
  These are general math functions and constants.
  Alot of them are simple stub functions to allow
  for easy overriding if required.
*/


#include "../detail/detail.hpp"
#include "constants.hpp"
#include <math.h>
#include <cmath>
#include <algorithm>
#include <random>


// --------------------------------------------------------------- [ Config ] --


#define MATH_GENR_INLINE MATH_INLINE


_MATH_NS_OPEN


// ----------------------------------------------------------------- [ Trig ] --


MATH_GENR_INLINE float              tan(const float x);
MATH_GENR_INLINE float              a_tan2(const float x, const float y);
MATH_GENR_INLINE float              cos(const float radians);
MATH_GENR_INLINE float              a_cos(const float radians);
MATH_GENR_INLINE float              sin(const float radians);
MATH_GENR_INLINE float              a_sin(const float radians);


// ---------------------------------------------------- [ Degrees / Radians ] --


MATH_GENR_INLINE float              to_degrees(const float radians);
MATH_GENR_INLINE float              to_radians(const float degrees);


// -------------------------------------------------------------- [ Casting ] --


MATH_GENR_INLINE float              to_float(const uint32_t x);
MATH_GENR_INLINE float              to_float(const int32_t x);
MATH_GENR_INLINE int32_t            to_int(const float x);
MATH_GENR_INLINE uint32_t           to_uint(const float x);


// --------------------------------------------------------------- [ Random ] --


MATH_GENR_INLINE float              rand_range(const float start, const float end);
MATH_GENR_INLINE uint32_t           rand_range(const uint32_t start, const uint32_t end);
MATH_GENR_INLINE int32_t            rand_range(const int32_t start, const int32_t end);


// ------------------------------------------------------------ [ Min / Max ] --


MATH_GENR_INLINE float              max(const float a, const float b);
MATH_GENR_INLINE float              min(const float a, const float b);

MATH_GENR_INLINE int32_t            max(const int32_t a, const int32_t b);
MATH_GENR_INLINE int32_t            min(const int32_t a, const int32_t b);

MATH_GENR_INLINE uint32_t           max(const uint32_t a, const uint32_t b);
MATH_GENR_INLINE uint32_t           min(const uint32_t a, const uint32_t b);

MATH_GENR_INLINE uint64_t           min(const uint64_t a, const uint64_t b);

MATH_GENR_INLINE void               max(float *out, const float *a, const float *b, const size_t count);
MATH_GENR_INLINE void               min(float *out, const float *a, const float *b, const size_t count);


// ------------------------------------------------------------- [ Rounding ] --


MATH_GENR_INLINE float              ceil(const float x);
MATH_GENR_INLINE float              floor(const float x);
MATH_GENR_INLINE float              nearest_floor(const float x, const float increments);
MATH_GENR_INLINE float              clamp(const float x, const float between_a, const float between_b);


// ----------------------------------------------------------- [ Arithmetic ] --


MATH_GENR_INLINE void               add(float *out, const float *a, const float *b, const size_t count);


// ----------------------------------------------------------------- [ Misc ] --


MATH_GENR_INLINE float              max_length(const float a, const float b);
MATH_GENR_INLINE float              min_length(const float a, const float b);


MATH_GENR_INLINE float              abs(const float x);
MATH_GENR_INLINE int32_t            abs(const int32_t x);

MATH_GENR_INLINE float              sqrt(const float x);

MATH_GENR_INLINE float              sign(const float x);
MATH_GENR_INLINE float              mod(const float x, const float divisor);


// ------------------------------------------------------------- [ Equality ] --


MATH_GENR_INLINE bool               is_between(const float to_check, const float a, const float b);
MATH_GENR_INLINE bool               is_near(const float a, const float b, const float error_margin = math::epsilon());
MATH_GENR_INLINE bool               is_pow_two(const uint32_t i);


// ----------------------------------------------------------------- [ Trig ] --


float
tan(const float x)
{
  return tanf(x);
}


float
a_tan2(const float x, const float y)
{
  return atan2f(x, y);
}


float
cos(const float radians)
{
  return cosf(radians);
}


float
a_cos(const float radians)
{
  return acosf(radians);
}


float
sin(const float radians)
{
  return sinf(radians);
}


float
a_sin(const float radians)
{
  return asinf(radians);
}


// ---------------------------------------------------- [ Degrees / Radians ] --


float
to_degrees(const float radians)
{
  constexpr float over_pi = 180.f / pi();
  return radians * over_pi;
}


float
to_radians(const float degrees)
{
  constexpr float over_180 = pi() / 180.f;
  return degrees * over_180;
}


// -------------------------------------------------------------- [ Casting ] --


float
to_float(const uint32_t x)
{
  return (float)x;
}


float
to_float(const int32_t x)
{
  return (float)x;
}


int32_t
to_int(const float x)
{
  return (int32_t)x;
}


uint32_t
to_uint(const float x)
{
  return (uint32_t)x;
}


// --------------------------------------------------------------- [ Random ] --


float
rand_range(const float start, const float end)
{
  // static std::random_device rd;
  // static std::default_random_engine re(rd());

  // std::uniform_real_distribution<float> dist(start, end);

  // return dist(re);

  const uint32_t range = (MATH_NS_NAME::max(start,end) - MATH_NS_NAME::min(start, end)) * 1000.f;
  const uint32_t rand_value = rand() % range;

  return MATH_NS_NAME::min(start, end) + MATH_NS_NAME::to_float(rand_value) / 1000.f;
}


uint32_t
rand_range(const uint32_t start, const uint32_t end)
{
  static std::random_device rd;
  static std::default_random_engine re(rd());

  std::uniform_int_distribution<uint32_t> dist(start, end);

  return dist(re);
}


int32_t
rand_range(const int32_t start, const int32_t end)
{
  static std::random_device rd;
  static std::default_random_engine re(rd());

  std::uniform_int_distribution<int32_t> dist(start, end);

  return dist(re);
}

// ------------------------------------------------------------ [ Min / Max ] --


float
max(const float x, const float y)
{
  return fmaxf(x, y);
}


int32_t
max(const int32_t x, const int32_t y)
{
  return std::max(x, y);
}


uint32_t
max(const uint32_t x, const uint32_t y)
{
  return std::max(x, y);
}


float
min(const float x, const float y)
{
  return fminf(x, y);
}


int32_t
min(const int32_t x, const int32_t y)
{
  return std::min(x, y);
}


uint32_t
min(const uint32_t x, const uint32_t y)
{
  return std::min(x, y);
}


uint64_t
min(const uint64_t x, const uint64_t y)
{
  return std::min(x, y);
}


void
max(float *out, const float *a, const float *b, const size_t count)
{
  // -- Param Check -- //
  MATH_ASSERT(out);
  MATH_ASSERT(a);
  MATH_ASSERT(b);

  // -- Add Array together -- //
  for(size_t i = 0; i < count; ++i)
  {
    out[i] = max(a[i], b[i]);
  }
}


void
min(float *out, const float *a, const float *b, const size_t count)
{
  // -- Param Check -- //
  MATH_ASSERT(out);
  MATH_ASSERT(a);
  MATH_ASSERT(b);

  // -- Add Array together -- //
  for(size_t i = 0; i < count; ++i)
  {
    out[i] = min(a[i], b[i]);
  }
}


// ------------------------------------------------------------- [ Rounding ] --


float
clamp(const float x, const float between_a, const float between_b)
{
  const float highest = between_a >= between_b ? between_a : between_b;
  const float lowest  = between_a != highest ? between_a : between_b;

  if(x >= highest)
  {
    return highest;
  }
  else if(x <= lowest)
  {
    return lowest;
  }

  return x;
}


float
ceil(const float x)
{
  return ((float)(long)((x)+1));
}


float
floor(const float x)
{
  return ((float)(long)x);
}


float
nearest_floor(const float x, const float increments)
{
  const float remainder = mod(x, increments);
  return x - remainder;
}


// ----------------------------------------------------------- [ Arithmetic ] --


void
add(float *out, const float *a, const float *b, const size_t count)
{
  // -- Param Check -- //
  MATH_ASSERT(out);
  MATH_ASSERT(a);
  MATH_ASSERT(b);

  // -- Add Array together -- //
  for(size_t i = 0; i < count; ++i)
  {
    out[i] = a[i] + b[i];
  }
}


// ----------------------------------------------------------------- [ Misc ] --


float
max_length(const float a, const float b)
{
  if(MATH_NS_NAME::abs(a) >= MATH_NS_NAME::abs(b))
  {
    return a;
  }

  return b;
}


float
min_length(const float a, const float b)
{
  if(MATH_NS_NAME::abs(a) <= MATH_NS_NAME::abs(b))
  {
    return a;
  }

  return b;
}


float
sqrt(const float x)
{
  return sqrtf(x);
}


float
abs(const float x)
{
  return fabsf(x);
}


int32_t
abs(const int32_t x)
{
  return std::abs(x);
}


float
sign(const float x)
{
  return x >= 0.f ? +1.f : -1.f;
}


float
mod(const float x, const float divisor)
{
  return fmodf(x, divisor);
}


// ------------------------------------------------------------- [ Equality ] --


bool
is_between(const float value, const float limit_a, const float limit_b)
{
  const float min = MATH_NS_NAME::min(limit_a, limit_b);
  const float max = MATH_NS_NAME::max(limit_a, limit_b);

  return value > min && value < max;
}


bool
is_pow_two(const uint32_t i)
{
  return (i & (i - 1)) == 0;
}


bool
is_near(const float a, const float b, const float error_margin)
{
  const float abs_error = abs(error_margin);

  return abs(b - a) <= abs_error;
}


_MATH_NS_CLOSE


#endif // include guard
