#ifndef FUNDAMENTAL_INCLUDED_C3B2733E_2BD0_47C9_97D8_1EEDD88B1AB5
#define FUNDAMENTAL_INCLUDED_C3B2733E_2BD0_47C9_97D8_1EEDD88B1AB5


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------------- [ fixed ints ] -- */


#if defined(_MSC_VER) && !defined(__cplusplus)
  typedef signed   __int8  int8_t;
  typedef unsigned __int8  uint8_t;
  typedef signed   __int16 int16_t;
  typedef unsigned __int16 uint16_t;
  typedef signed   __int32 int32_t;
  typedef unsigned __int32 uint32_t;
  typedef signed   __int64 int64_t;
  typedef unsigned __int64 uint64_t;
#else
  #include <stdint.h>
#endif


/* ----------------------------------------------------- [ array helpers ] -- */


#define ROA_ARR_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))
#define ROA_ARR_DATA(arr) (&arr[0])


/* ---------------------------------------------------- [ config helpers ] -- */


#define ROA_IS_ENABLED(expr) ((expr) != 0)


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */