#ifndef FUNDAMENTAL_INCLUDED_C3B2733E_2BD0_47C9_97D8_1EEDD88B1AB5
#define FUNDAMENTAL_INCLUDED_C3B2733E_2BD0_47C9_97D8_1EEDD88B1AB5


/* ---------------------------------------------------- [ fixed integers ] -- */


#ifndef __cplusplus
	#ifdef _MSC_VER
		typedef signed   __int8  int8_t;
		typedef unsigned __int8  uint8_t;
		typedef signed   __int16 int16_t;
		typedef unsigned __int16 uint16_t;
		typedef signed   __int32 int32_t;
		typedef unsigned __int32 uint32_t;
		typedef signed   __int64 int64_t;
		typedef unsigned __int64 uint64_t;
		#include <stddef.h>
	#else
		#include <stdint.h>
		#include <stddef.h>
	#endif
#else
	#include <cstdint>
	#include <cstddef>
#endif


/* ------------------------------------------------------- [ basic types ] -- */


#define ROA_NULL 0

typedef int ROA_BOOL;
#define ROA_TRUE 1
#define ROA_FALSE 0


/* ----------------------------------------------------- [ array helpers ] -- */


#define ROA_ARR_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))
#define ROA_ARR_DATA(arr) (&arr[0])


/* ---------------------------------------------------- [ config helpers ] -- */


#define ROA_IS_ENABLED(expr) ((expr) != 0)


/* ------------------------------------------------------- [ conversions ] -- */


#define ROA_MB_TO_BYTES(mb) (mb * 1048576)


/* -------------------------------------------------------------- [ bits ] -- */


#define ROA_BIT(ui) 1 << ui

#define ROA_UPPER_32_BITS(ui64) (ui64 & 0xFFFFFFFF)
#define ROA_LOWER_32_BITS(ui64) (ui64 >> 32)
#define ROA_FIRST8_BITS(ui32) (ui32 >> 24 & 0xFF)
#define ROA_SECOND8_BITS(ui32) (ui32 >> 16 & 0xFF)
#define ROA_THIRD8_BITS(ui32) (ui32 >> 8 & 0xFF)
#define ROA_FORTH8_BITS(ui32) (ui32 >> & 0xFFFFFF)
#define ROA_UPPER_24_BITS(ui32) (ui32 & 0xFFFFFF)

#define ROA_PACK3232(ui32a, ui32b) ((uint64_t)ui32a) << 32 | ui32b
#define ROA_PACK8888(ui8a, ui8b, ui8c, ui8d) (uint32_t)((uint32_t)ui8a << 24) | ((uint32_t)ui8b << 16) | ((uint32_t)ui8c << 8) | ((uint32_t)ui8d << 0))
#define ROA_PACK824(ui8a, ui32b) (uint32_t)(ui8a << 24) | ui32b


/* ------------------------------------------------------ [ util helpers ] -- */


#define ROA_UNUSED(x) (void)(x)
#define ROA_MEM_ZERO(var) do { unsigned i; for(i = 0; i < sizeof(var); ++i) {((unsigned char *) &var)[i] = 0;} } while(0)
#define ROA_MEM_CPY(dst, src) do { unsigned i; for(i = 0; i < sizeof(src); ++i) { ((unsigned char*) dst)[i] = ((unsigned char*) src)[i]; } } while(0)
#define ROA_STRINGIFY(a) _ROA_STRINGIFY(a)
#define _ROA_STRINGIFY(a) #a

#endif /* inc guard */
