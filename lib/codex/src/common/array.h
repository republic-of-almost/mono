#ifndef ARRAY_INCLUDED_A77F4BD2_DE1B_48D3_86CA_41294DE06DE0
#define ARRAY_INCLUDED_A77F4BD2_DE1B_48D3_86CA_41294DE06DE0


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


#define codex_array_create(arr, cap) \
  array_grow((void**)&arr, sizeof(arr[0]), cap)

#define codex_array_size(arr) \
  array_size((void**)&arr)

#define codex_array_capacity(arr) \
  array_capacity((void**)&arr)

#define codex_array_push(arr, item) \
  array_should_grow((void**)&arr);  \
  arr[array_size((void**)&arr)] = item;  \
  array_increment((void**)&arr);

#define codex_array_erase(arr, index) \
  array_erase((void**)&arr, index);

#define codex_array_destroy(arr) \
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
