#ifndef ARRAY_INCLUDED_8DEFA288_CEAB_4CBA_A7BE_FAF80132D7C1
#define ARRAY_INCLUDED_8DEFA288_CEAB_4CBA_A7BE_FAF80132D7C1


#ifdef __cplusplus
extern "C" {
#endif


#define ROA_ARRAY_COUNT(arr) sizeof(arr) / sizeof(arr[0])
#define ROA_ARRAY_PTR(arr) &arr[0]


/* --------------------------------------------------- [ Array Interface ] -- */
/*
C is a little awkward for containers, but this is a good balance for arrays.
It uses macros unforunatly but it means we have a typesafe buffer.

int *array = NULL;
codex_array_create(array);
codex_array_push(array, 123);
*/


#define roa_array_create(arr, cap) \
  roa_internal_array_grow((void**)&arr, sizeof(arr[0]), cap)

#define roa_array_size(arr) \
  roa_internal_array_size((void**)&arr)

#define roa_array_resize(arr, cap) \
  roa_internal_array_resize((void**)&arr, cap)

#define roa_array_capacity(arr) \
  roa_internal_array_capacity((void**)&arr)

#define roa_array_push(arr, item) \
  roa_internal_array_should_grow((void**)&arr);  \
  arr[roa_internal_array_size((void**)&arr)] = item;  \
  roa_internal_array_increment((void**)&arr);

#define roa_array_erase(arr, index) \
  roa_internal_array_erase((void**)&arr, index);

#define roa_array_destroy(arr) \
  roa_internal_array_destroy((void**)&arr); \
  arr = NULL

#define roa_array_pop(arr) \
  roa_internal_array_internal_pop((void**)&arr); \

#define roa_array_back(arr) \
  arr[roa_array_size(arr) - 1] \


/* -------------------------------------------------------- [ Array Impl ] -- */
/*
These are internal you shouldn't be calling them directly.
*/

void      roa_internal_array_resize(void **ptr, unsigned size);
void      roa_internal_array_destroy(void **ptr);
void      roa_internal_array_grow(void **ptr, unsigned stride, unsigned capacity);
unsigned  roa_internal_array_size(void **ptr);
unsigned  roa_internal_array_capacity(void **ptr);
void      roa_internal_array_increment(void **ptr);
void      roa_internal_array_should_grow(void **ptr);
void      roa_internal_array_erase(void **ptr, unsigned index);
void      roa_internal_array_internal_pop(void **ptr);



#ifdef __cplusplus
} /* extern */
#endif

#endif /* inc guard */