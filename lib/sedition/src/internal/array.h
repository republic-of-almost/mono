#ifndef ARRAY_INCLUDED_6CA0EFC6_C106_4715_A8E9_A2345DD2ACC6
#define ARRAY_INCLUDED_6CA0EFC6_C106_4715_A8E9_A2345DD2ACC6


#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


/* --------------------------------------------------- [ Array Interface ] -- */
/*
  C is a little awkward for containers, but this is a good balance for arrays.
  It uses macros unforunatly but it means we have a typesafe buffer.
  
  int *array = NULL;
  codex_array_create(array);
  codex_array_push(array, 123);
*/


#define sed_array_create(arr, cap) \
  array_grow((void**)&arr, sizeof(arr[0]), cap)

#define sed_array_size(arr) \
  array_size((void**)&arr)

#define sed_array_capacity(arr) \
  array_capacity((void**)&arr)

#define sed_array_push(arr, item) \
  array_should_grow((void**)&arr);  \
  arr[array_size((void**)&arr)] = item;  \
  array_increment((void**)&arr);

#define sed_array_erase(arr, index) \
  array_erase((void**)&arr, index);

#define sed_array_destroy(arr) \
  array_destroy((void**)&arr); \
  arr = NULL


/* -------------------------------------------------------- [ Array Impl ] -- */
/*
  These are internal you shouldn't be calling them directly.
*/

void    array_destroy(void **ptr);
void    array_grow(void **ptr, uint32_t stride ,uint32_t capacity);
size_t  array_size(void **ptr);
size_t  array_capacity(void **ptr);
void    array_increment(void **ptr);
void    array_should_grow(void **ptr);
void    array_erase(void **ptr, size_t index);


#ifdef __cplusplus
}
#endif


#endif /* inc guard */
