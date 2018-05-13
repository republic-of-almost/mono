
#include <common/alloc.h>
#include <stdlib.h>
#include <string.h>


void *
rdr_alloc(unsigned bytes)
{
  return calloc(bytes, 1);
}


void
rdr_free(void *addr)
{
  free(addr);
}


void *
rdr_frame_alloc(unsigned bytes)
{
  return 0;
}


void *
rdr_alloc_copy(void *src, unsigned size)
{
  if(src)
  {
    void *dst = malloc(size);
    memcpy(dst, src, size);

    return dst;
  }
  else
  {
    return 0;
  }
}