#include <roa_lib/array.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <string.h>
#include <stdlib.h>


/* -------------------------------------------------------- [ Array Impl ] -- */


struct roa_array_internal
{
  unsigned char * __capacity;
  unsigned char * __end;

  /* deprecated */
  unsigned stride;
  unsigned capacity;
  unsigned count;
};


void*
roa_internal_array_create(void **ptr, unsigned stride, unsigned capacity)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr == ROA_NULL);

  size_t bytes = sizeof(struct roa_array_internal) + (stride * capacity);
  struct roa_array_internal *new_arr = roa_zalloc(bytes);

  new_arr[0].__end = (unsigned char*)new_arr;
  new_arr[0].__capacity = new_arr[0].__end += bytes;

  *ptr = &new_arr[1];

  return new_arr;
}


void
roa_internal_array_destroy(void **ptr)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr != ROA_NULL);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  roa_free(curr_arr);
  *ptr = NULL;
}


unsigned
roa_internal_array_size(void **ptr, unsigned stride)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);
  ROA_ASSERT(stride);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  return (curr_arr->__end - ((unsigned char*)*ptr)) / stride;
}


unsigned
roa_internal_array_capacity(void **ptr, unsigned stride)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);
  ROA_ASSERT(stride);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  return (curr_arr->__capacity - ((unsigned char*)*ptr)) / stride;
}


void
roa_internal_array_resize(void **ptr, unsigned size, unsigned stride)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);
  ROA_ASSERT(size);
  ROA_ASSERT(stride);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  curr_arr = roa_internal_array_grow(ptr, curr_arr->stride, size);
  curr_arr->__end = curr_arr->__capacity;
}


void
roa_internal_array_erase(void **ptr, unsigned index)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  unsigned char *dst = &((unsigned char*)(*ptr))[curr_arr->stride * index];
  unsigned char *src = &((unsigned char*)(*ptr))[curr_arr->stride * (index + 1)];

  curr_arr->count -= 1;

  size_t len = (curr_arr->stride * (curr_arr->count)) - (curr_arr->stride * index);

  memmove(dst, src, len);
}


void
roa_internal_array_pop(void **ptr)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  if (curr_arr->count > 0)
  {
    curr_arr->count -= 1;
  }  
}


void
roa_internal_array_clear(void**ptr)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  curr_arr->count = 0;
}


void*
roa_internal_array_grow(void **ptr, unsigned stride, unsigned capacity)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);
  ROA_ASSERT(stride);
  ROA_ASSERT(capacity);

  /* increase buffer */
  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  size_t old_count = curr_arr->count;
  size_t bytes = (stride * capacity) + sizeof(struct roa_array_internal);
  curr_arr = (struct roa_array_internal*)realloc(curr_arr, bytes);
  curr_arr[0].count = old_count;
  curr_arr[0].capacity = capacity;
  curr_arr[0].stride = stride;

  *ptr = &curr_arr[1];

  return curr_arr;
}


unsigned
roa_internal_array_push(void **ptr, unsigned stride)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);
  ROA_ASSERT(stride);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;
  
  if (count > roa_internal_array_capacity(ptr, stride))
  {
    roa_internal_array_grow(ptr, curr_arr->stride, curr_arr->count * 2);
  }
}

void
roa_internal_array_should_grow(void **ptr, unsigned stride)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  unsigned count = curr_arr->count + 1;

  if (count > roa_internal_array_capacity(ptr, stride))
  {
    roa_internal_array_grow(ptr, curr_arr->stride, curr_arr->count * 2);
  }
}
