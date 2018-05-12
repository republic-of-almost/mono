
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


void * rdr_frame_alloc(unsigned bytes)
{
  return 0;
}
