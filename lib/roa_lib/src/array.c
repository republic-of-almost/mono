#include <roa_lib/array.h>
#include <roa_lib/alloc.h>
#include <roa_lib/assert.h>
#include <string.h>
#include <stdlib.h>


/* ------------------------------------------------------ [ Array Config ] -- */


/* turn this on if you suspect something in array */
#define ROA_ARR_PEDANTIC_CHECKS 0


void* roa_internal_array_grow(void **ptr, unsigned stride, unsigned capacity);
void roa_internal_array_should_grow(void **ptr, unsigned stride);


/* -------------------------------------------------------- [ Array Impl ] -- */


struct roa_array_header
{
  unsigned char * capacity;
  unsigned char * end;
};


void
roa_internal_array_create(void **ptr, unsigned stride, unsigned capacity)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr == ROA_NULL);
  }

  if (capacity == 0)
  {
    capacity = 1;
  }

  unsigned array_bytes = stride * capacity;
  unsigned bytes = sizeof(struct roa_array_header) + array_bytes;

  struct roa_array_header *header = malloc(bytes);
  unsigned char *begin = (unsigned char*)&header[1];

  header[0].end = begin;
  header[0].capacity = begin + array_bytes;

  *ptr = (void*)begin;
}


void
roa_internal_array_destroy(void **ptr)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
  }

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  roa_free(header);
  *ptr = NULL;
}


unsigned
roa_internal_array_size(void **ptr, unsigned stride)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
    ROA_ASSERT(stride);
  }

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  unsigned char * begin = ((unsigned char*)*ptr);
  unsigned count = (header->end - begin) / stride;

  return count;
}


unsigned
roa_internal_array_capacity(void **ptr, unsigned stride)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
    ROA_ASSERT(stride);
  }

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  return (header->capacity - ((unsigned char*)*ptr)) / stride;
}


void
roa_internal_array_resize(void **ptr, unsigned size, unsigned stride)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
    ROA_ASSERT(size);
    ROA_ASSERT(stride);
  }

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  header = roa_internal_array_grow(ptr, stride, size);
  header->end = header->capacity;
}


void
roa_internal_array_erase(void **ptr, unsigned index, unsigned stride)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
    ROA_ASSERT(stride);
  }

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  unsigned char *dst = &((unsigned char*)(*ptr))[stride * index];
  unsigned char *src = &((unsigned char*)(*ptr))[stride * (index + 1)];

  unsigned count = roa_internal_array_size(ptr, stride);

  if (count > 0)
  {
    unsigned len = (stride * (count - 1)) - (stride * index);
    memmove(dst, src, len);

    header->end -= stride;
  }
}


void
roa_internal_array_pop(void **ptr, unsigned stride)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
  }

  unsigned char *begin = (unsigned char*)*ptr;

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  if (begin < header->end)
  {
    header->end -= stride;
  }
}


void
roa_internal_array_clear(void**ptr)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
  }

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  unsigned char *begin = (unsigned char*)&header[1];

  header->end = begin;
}


void*
roa_internal_array_grow(void **ptr, unsigned stride, unsigned capacity)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
    ROA_ASSERT(stride);
    ROA_ASSERT(capacity);
  }

  /* increase buffer */
  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  unsigned count = roa_internal_array_size(ptr, stride);
  unsigned bytes = (stride * capacity) + sizeof(struct roa_array_header);

  header = (struct roa_array_header*)realloc(header, bytes);

  unsigned char *begin = (unsigned char*)&header[1];

  header->end = begin + (stride * count);
  header->capacity = begin + (stride * capacity);

  *ptr = &header[1];

  return header;
}


unsigned
roa_internal_array_push(void **ptr, unsigned stride)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
    ROA_ASSERT(stride);
  }

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  if (header->end >= header->capacity)
  {
    unsigned count = roa_internal_array_size(ptr, stride);
    header = (struct roa_array_header *)roa_internal_array_grow(
      ptr,
      stride,
      count * 2
    );
  }

  unsigned char * begin = (unsigned char*)(*ptr);

  /* increment end */
  unsigned index = (header->end - begin) / stride;
  header->end += stride;

  return index;
}


unsigned
roa_internal_array_insert(void **ptr, unsigned index, unsigned stride)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
    ROA_ASSERT(stride);
  }

  /* increase the size to make space */
  roa_internal_array_push(ptr, stride);

  struct roa_array_header *header = ((struct roa_array_header*)*ptr);
  header--;

  unsigned count = roa_internal_array_size(ptr, stride);

  unsigned char * begin = (unsigned char*)(*ptr);

  unsigned char *dst = &begin[stride * (index + 1)];
  unsigned char *src = &begin[stride * index];

  unsigned len = (count - index - 1) * stride;

  memmove(dst, src, len);

  return index;
}


void
roa_internal_array_should_grow(void **ptr, unsigned stride)
{
  if (ROA_IS_ENABLED(ROA_ARR_PEDANTIC_CHECKS))
  {
    ROA_ASSERT(ptr);
    ROA_ASSERT(*ptr);
    ROA_ASSERT(stride);
  }

  struct roa_array_header *curr_arr = ((struct roa_array_header*)*ptr);
  curr_arr--;

  unsigned count = roa_internal_array_size(ptr, stride) + 1;

  if (count > roa_internal_array_capacity(ptr, stride))
  {
    roa_internal_array_grow(ptr, stride, count * 2);
  }
}


/* ------------------------------------------------------ [ Array Config ] -- */


#undef ROA_ARR_PEDANTIC_CHECKS