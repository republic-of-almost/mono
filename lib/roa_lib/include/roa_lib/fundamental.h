#ifndef FUNDAMENTAL_INCLUDED_C3B2733E_2BD0_47C9_97D8_1EEDD88B1AB5
#define FUNDAMENTAL_INCLUDED_C3B2733E_2BD0_47C9_97D8_1EEDD88B1AB5


#ifdef __cplusplus
extern "C" {
#endif


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
#endif
#else
#include <stdint.h>
#include <stddef.h>
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


/* ------------------------------------------------------ [ util helpers ] -- */


#define ROA_UNUSED(x) (void)(x)
#define ROA_MEM_ZERO(var) do { unsigned i; for(i = 0; i < sizeof(var); ++i) {((unsigned char *) &var)[i] = 0;} } while(0)
#define ROA_MEM_CPY(dst, src) do { unsigned i; for(i = 0; i < sizeof(src); ++i) { ((unsigned char*) dst)[i] = ((unsigned char*) src)[i]; } } while(0);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */