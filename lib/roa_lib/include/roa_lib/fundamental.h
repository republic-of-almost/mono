#ifndef FUNDAMENTAL_INCLUDED_C3B2733E_2BD0_47C9_97D8_1EEDD88B1AB5
#define FUNDAMENTAL_INCLUDED_C3B2733E_2BD0_47C9_97D8_1EEDD88B1AB5


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------------- [ fixed ints ] -- */





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


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */