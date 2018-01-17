#include <roa_lib/array.h>
#include <stdlib.h>
#include <string.h>


/* -------------------------------------------------------- [ Array Impl ] -- */


struct roa_array_internal
{
  unsigned count;
  unsigned capacity;
  unsigned stride;
};


void
roa_internal_array_destroy(void **ptr)
{
  if (*ptr)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    free(curr_arr);
  }
}


void
roa_internal_array_resize(void **ptr, unsigned size)
{
  if (*ptr)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    roa_internal_array_grow(ptr, curr_arr->stride, size);
    curr_arr->count = size;
  }
}


void
roa_internal_array_grow(void **ptr, unsigned stride, unsigned capacity)
{
  /* increase buffer */
  if (*ptr != NULL)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    size_t old_count = curr_arr->count;
    size_t bytes = (stride * capacity) + sizeof(struct roa_array_internal);
    curr_arr = (struct roa_array_internal*)realloc(curr_arr, bytes);
    curr_arr[0].count = old_count;
    curr_arr[0].capacity = capacity;
    curr_arr[0].stride = stride;

    *ptr = &curr_arr[1];
  }
  /* new */
  else
  {
    size_t bytes = sizeof(struct roa_array_internal) + (stride * capacity);
    struct roa_array_internal *new_arr = (struct roa_array_internal*)malloc(bytes);
    new_arr[0].count = 0;
    new_arr[0].capacity = capacity;
    new_arr[0].stride = stride;

    *ptr = &new_arr[1];
  }
}

unsigned
roa_internal_array_size(void **ptr)
{
  if (*ptr)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    return curr_arr->count;
  }
  else
  {
    return 0;
  }
}


unsigned
roa_internal_array_capacity(void **ptr)
{
  if (*ptr)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    return curr_arr->capacity;
  }
  else
  {
    return 0;
  }
}

void
roa_internal_array_increment(void **ptr)
{
  if (*ptr)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    curr_arr->count++;
  }
}

void
roa_internal_array_should_grow(void **ptr)
{
  if (*ptr)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    unsigned count = curr_arr->count + 1;

    if (count > roa_internal_array_capacity(ptr))
    {
      roa_internal_array_grow(ptr, curr_arr->stride, curr_arr->count * 2);
    }
  }
}


void
roa_internal_array_erase(void **ptr, unsigned index)
{
  if (*ptr)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    unsigned char *dst = &((unsigned char*)(*ptr))[curr_arr->stride * index];
    unsigned char *src = &((unsigned char*)(*ptr))[curr_arr->stride * (index + 1)];

    curr_arr->count -= 1;

    size_t len = (curr_arr->stride * (curr_arr->count)) - (curr_arr->stride * index);

    memmove(dst, src, len);
  }
}


void
roa_internal_array_internal_pop(void **ptr)
{
  if (*ptr)
  {
    struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
    curr_arr--;

    if (curr_arr->count > 0)
    {
      curr_arr->count -= 1;
    }
  }
}
