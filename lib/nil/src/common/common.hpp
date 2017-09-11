#ifndef NIL_COMMON_INCLUDED_3567BECD_5592_4853_8C6F_0732D54CE325
#define NIL_COMMON_INCLUDED_3567BECD_5592_4853_8C6F_0732D54CE325


#include <stddef.h>


namespace Nil_detail {


// ------------------------------------------------------------ [ Copy Data ] --


using data_cpy_allocator = void*(*)(size_t length);


bool
copy_data_name(
  char *dest,
  const char *src,
  data_cpy_allocator alloc);


bool
copy_data(
  void *dest,
  void *src,
  size_t data_size,
  data_cpy_allocator alloc);


} // ns


#endif // inc guard
