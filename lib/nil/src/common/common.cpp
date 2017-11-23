#include <common/common.hpp>
#include <string.h>
#include <deque>

namespace Nil_detail {


bool
copy_data_name(
  char **dest,
  const char *src,
  data_cpy_allocator alloc)
{
  const size_t data_size = sizeof(char) * (strlen(src) + 1);
  *dest = (char*)alloc(data_size);
  
  if(*dest)
  {
    memset(*dest, 0, data_size);
    memcpy(*dest, src, data_size);
    
    return true;
  }
  
  return false;
}


bool
copy_data(
  void **dest,
  void *src,
  size_t data_size,
  data_cpy_allocator alloc)
{
  *dest = alloc(data_size);
  
  if(*dest)
  {
    memset(*dest, 0, data_size);
    memcpy(*dest, src, data_size);
    
    return true;
  }
  
  return false;
}


} // ns
