#ifndef ARRAY_INCLUDED_A77F4BD2_DE1B_48D3_86CA_41294DE06DE0
#define ARRAY_INCLUDED_A77F4BD2_DE1B_48D3_86CA_41294DE06DE0


#include <stdlib.h>


/* --------------------------------------------------- [ Array Internals ] -- */


struct Array
{
  size_t size;
  size_t capacity;
};


/* --------------------------------------------------- [ Array Interface ] -- */


#define codex_array_create_with_size(arr, init_size) \
  malloc()

#define codex_array_create(arr) codex_array_create_with_size(arr, 0)

#define codex_array_destroy(arr)
#define codex_array_push(arr, item)
#define codex_array_resize(arr, count)
#define codex_array_pop(arr, item)
#define codex_array_erase(arr, index)


/* -------------------------------------------------------- [ Array Impl ] -- */





#endif /* inc guard */
