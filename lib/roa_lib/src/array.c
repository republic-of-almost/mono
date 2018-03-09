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
};


void*
roa_internal_array_create(void **ptr, unsigned stride, unsigned capacity)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr == ROA_NULL);

  if (capacity == 0)
  {
    capacity = 1;
  }

  unsigned array_bytes = (stride * capacity);
  unsigned bytes = sizeof(struct roa_array_internal) + array_bytes;

  struct roa_array_internal *new_arr = malloc(bytes);
  unsigned char *__begin = (unsigned char*)&new_arr[1];

  new_arr[0].__end = __begin;
  new_arr[0].__capacity = __begin + array_bytes;

  *ptr = new_arr[0].__end;

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

  unsigned char * __begin = ((unsigned char*)*ptr);
  unsigned count = (curr_arr->__end - __begin) / stride;

  return count;
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

  curr_arr = roa_internal_array_grow(ptr, stride, size);
  curr_arr->__end = curr_arr->__capacity;
}


void
roa_internal_array_erase(void **ptr, unsigned index, unsigned stride)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  unsigned char *dst = &((unsigned char*)(*ptr))[stride * index];
  unsigned char *src = &((unsigned char*)(*ptr))[stride * (index + 1)];

  unsigned count = roa_internal_array_size(ptr, stride);

  if (count > 0)
  {
    unsigned len = (stride * (count - 1)) - (stride * index);
    memmove(dst, src, len);

    curr_arr->__end -= stride;
  }
}


void
roa_internal_array_pop(void **ptr, unsigned stride)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);

  unsigned char *__begin = (unsigned char*)*ptr;

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  if (__begin < curr_arr->__end)
  {
    curr_arr->__end -= stride;
  }  
}


void
roa_internal_array_clear(void**ptr)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  unsigned char *__begin = (unsigned char*)&curr_arr[1];
  
  curr_arr->__end = __begin;
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

  unsigned count = roa_internal_array_size(ptr, stride);
  unsigned bytes = (stride * capacity) + sizeof(struct roa_array_internal);

  curr_arr = (struct roa_array_internal*)realloc(curr_arr, bytes);

  unsigned char *__begin = (unsigned char*)&curr_arr[1];

  curr_arr->__end = __begin + (stride * count);
  curr_arr->__capacity = __begin + (stride * capacity);

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

  if (curr_arr->__end >= curr_arr->__capacity)
  {
    unsigned count = roa_internal_array_size(ptr, stride);
    curr_arr = (unsigned char *)roa_internal_array_grow(ptr, stride, count * 2);
  }


  unsigned char * __begin = (unsigned char*)(*ptr);

  /* increment end */
  unsigned index = (curr_arr->__end - __begin) / stride;
  curr_arr->__end += stride;

  return index;
}


unsigned
roa_internal_array_insert(void **ptr, unsigned index, unsigned stride)
{
  ROA_ASSERT(0); /* do insert */

  return 0;
}

void
roa_internal_array_should_grow(void **ptr, unsigned stride)
{
  ROA_ASSERT(ptr);
  ROA_ASSERT(*ptr);

  struct roa_array_internal *curr_arr = ((struct roa_array_internal*)*ptr);
  curr_arr--;

  unsigned count = roa_internal_array_size(ptr, stride) + 1;

  if (count > roa_internal_array_capacity(ptr, stride))
  {
    roa_internal_array_grow(ptr, stride, count * 2);
  }
}
