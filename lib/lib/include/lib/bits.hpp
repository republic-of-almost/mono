/*
  Bits
  --
  Anything todo with bits and bit packing.

  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef BITS_INCLUDED_CBA564C2_BA9A_4D4E_A0B8_BCAF9ECE089B
#define BITS_INCLUDED_CBA564C2_BA9A_4D4E_A0B8_BCAF9ECE089B


#include <stdint.h>


// ---------------------------------------------------------- [ Bits Config ] --


#ifndef LIB_BITS_INLINE
#define LIB_BITS_INLINE constexpr
#endif


#ifndef LIB_BITS_ASSERT
#include "assert.hpp"
#define LIB_BITS_ASSERT(expr) LIB_ASSERT(expr)
#endif


#ifndef LIB_NS_NAME
#define LIB_NS_NAME lib
#endif


// ------------------------------------------------------------ [ BIT Macro ] --


#define BIT(n) 1 << n


// --------------------------------------------------------- [ Bit Function ] --


namespace LIB_NS_NAME {
namespace bits {


LIB_BITS_INLINE size_t
bit(const uint32_t n)
{
  return 1 << n;
}


} // ns
} // ns


// ------------------------------------------------------- [ Bit Un-Packing ] --


namespace LIB_NS_NAME {
namespace bits {


LIB_BITS_INLINE uint32_t
upper32(const uint64_t i)
{
  return i & 0xFFFFFFFF;
}


LIB_BITS_INLINE uint32_t
lower32(const uint64_t i)
{
  return i >> 32;
}


LIB_BITS_INLINE uint8_t
first8(const uint32_t i)
{
  return i >> 24 & 0xFF;
}


LIB_BITS_INLINE uint8_t
second8(const uint32_t i)
{
  return i >> 16 & 0xFF;
}


LIB_BITS_INLINE uint8_t
third8(const uint32_t i)
{
  return i >> 8 & 0xFF;
}


LIB_BITS_INLINE uint8_t
forth8(const uint32_t i)
{
  return i & 0xFF;
}

LIB_BITS_INLINE uint32_t
upper24(const uint32_t i)
{
  return i & 0xFFFFFF;
}

} // ns
} // ns


// ---------------------------------------------------------- [ Bit Packing ] --


namespace LIB_NS_NAME {
namespace bits {


LIB_BITS_INLINE uint64_t
pack3232(const uint32_t a, const uint32_t b)
{
  return ((uint64_t)a) << 32 | b;
}


LIB_BITS_INLINE uint32_t
pack8888(const uint8_t a, const uint8_t b, const uint8_t c, const uint8_t d)
{
  return (uint32_t)(((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | ((uint32_t)d << 0));
}


LIB_BITS_INLINE uint32_t
pack824(const uint8_t a, const uint32_t b)
{
  LIB_BITS_ASSERT(b <= 0xFFFFFF);  //0 - 16777214 valid numbers only.
  
  return (a << 24) | b;
}


} // ns
} // ns


#endif // inc guard
