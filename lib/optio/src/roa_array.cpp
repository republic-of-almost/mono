#include <roa_array.h>
#include <stdlib.h>
#include <string.h>



/* -------------------------------------------------------- [ Array Impl ] -- */


struct codex_array_internal
{
  size_t count;
  size_t capacity;
  size_t stride;
};


void
array_destroy(void **ptr)
{
  if(*ptr)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;

    free(curr_arr);
  }
}


void
array_resize(void **ptr, size_t size)
{
  if(*ptr)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;
    
    array_grow(ptr, curr_arr->stride, size);
    curr_arr->count = size;
  }
}


void array_grow(void **ptr, size_t stride ,size_t capacity)
{
  /* increase buffer */
  if(*ptr != NULL)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;

    size_t old_count = curr_arr->count;
    size_t bytes = (stride * capacity) + sizeof(struct codex_array_internal);
    curr_arr = (struct codex_array_internal*)realloc(curr_arr, bytes);
    curr_arr[0].count = old_count;
    curr_arr[0].capacity = capacity;
    curr_arr[0].stride = stride;

    *ptr = &curr_arr[1];
  }
  /* new */
  else
  {
    size_t bytes = sizeof(struct codex_array_internal) + (stride * capacity);
    struct codex_array_internal *new_arr = (struct codex_array_internal*)malloc(bytes);
    new_arr[0].count = 0;
    new_arr[0].capacity = capacity;
    new_arr[0].stride = stride;

    *ptr = &new_arr[1];
  }
}

size_t array_size(void **ptr)
{
  if(*ptr)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;

    return curr_arr->count;
  }
  else
  {
    return 0;
  }
}


size_t array_capacity(void **ptr)
{
  if(*ptr)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;

    return curr_arr->capacity;
  }
  else
  {
    return 0;
  }
}

void array_increment(void **ptr)
{
  if(*ptr)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;

    curr_arr->count++;
  }
}

void array_should_grow(void **ptr)
{
  if(*ptr)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;

    size_t count = curr_arr->count + 1;

    if(count > array_capacity(ptr))
    {
      array_grow(ptr, curr_arr->stride, curr_arr->count * 2);
    }
  }
}


void
array_erase(void **ptr, size_t index)
{
  if(*ptr)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;

    uint8_t *dst = &((uint8_t*)(*ptr))[curr_arr->stride * index];
    uint8_t *src = &((uint8_t*)(*ptr))[curr_arr->stride * (index + 1)];
    
    curr_arr->count -= 1;
    
    size_t len = (curr_arr->stride * (curr_arr->count)) - (curr_arr->stride * index);

    memmove(dst, src, len);
  }
}


void
optio_arr_internal_pop(void **ptr)
{
  if(*ptr)
  {
    struct codex_array_internal *curr_arr = ((struct codex_array_internal*)*ptr);
    curr_arr--;
    
    if(curr_arr->count > 0)
    {
      curr_arr->count -= 1;
    }
  }
}
