#ifndef ARRAY_INCLUDED_EDF3D6C8_9DC0_44A0_B144_A7DF9BACA9BA
#define ARRAY_INCLUDED_EDF3D6C8_9DC0_44A0_B144_A7DF9BACA9BA


#include <stdint.h>
#include <stddef.h>


/* --------------------------------------------------- [ Array Interface ] -- */
/*
  C is a little awkward for containers, but this is a good balance for arrays.
  It uses macros unforunatly but it means we have a typesafe buffer.

  int *array = NULL;
  codex_array_create(array);
  codex_array_push(array, 123);
*/


#define optio_array_create(arr, cap) \
  array_grow((void**)&arr, sizeof(arr[0]), cap)

#define optio_array_size(arr) \
  array_size((void**)&arr)
  
#define optio_array_resize(arr, cap) \
  array_resize((void**)&arr, cap)

#define optio_array_capacity(arr) \
  array_capacity((void**)&arr)

#define optio_array_push(arr, item) \
  array_should_grow((void**)&arr);  \
  arr[array_size((void**)&arr)] = item;  \
  array_increment((void**)&arr);

#define optio_array_erase(arr, index) \
  array_erase((void**)&arr, index);

#define optio_array_destroy(arr) \
  array_destroy((void**)&arr); \
  arr = NULL
  
#define optio_array_pop(arr) \
  optio_arr_internal_pop((void**)&arr); \
  
#define optio_array_back(arr) \
  arr[optio_array_size(arr) - 1] \
  
  
/* -------------------------------------------------------- [ Array Impl ] -- */
/*
  These are internal you shouldn't be calling them directly.
*/

void    array_resize(void **ptr, size_t size);
void    array_destroy(void **ptr);
void    array_grow(void **ptr, size_t stride ,size_t capacity);
size_t  array_size(void **ptr);
size_t  array_capacity(void **ptr);
void    array_increment(void **ptr);
void    array_should_grow(void **ptr);
void    array_erase(void **ptr, size_t index);

void    optio_arr_internal_pop(void **ptr);


#endif /* inc guard */
