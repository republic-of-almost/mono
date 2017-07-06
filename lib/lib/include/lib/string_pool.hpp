/*
  String Pool
  --
  Convert strings to IDs.
  
  This is intended for things like tagging and faster comparissons etc.
  
  Todo
  --
  Currently just a very simplistic impl, will need another pass.

  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef STRING_POOL_INCLUDED_26A88090_79A3_42D1_8803_1895B52DAF00
#define STRING_POOL_INCLUDED_26A88090_79A3_42D1_8803_1895B52DAF00


#include <stdint.h>


// --------------------------------------------------- [ String Pool Config ] --


#ifndef LIB_STRING_POOL_MALLOC
#define LIB_STRING_POOL_MALLOC(type, count) (type*)malloc(sizeof(type) * count)
#endif


#ifndef LIB_STRING_POOL_FREE
#define LIB_STRING_POOL_FREE(ptr) free(ptr)
#endif



#ifndef LIB_NS_NAME
#define LIB_NS_NAME lib
#endif


// ------------------------------------------------- [ String Pool Interface] --


namespace LIB_NS_NAME {
namespace string_pool {


void
reset();


size_t
size();


uint32_t
add(const char *name);


uint32_t
find(const char *name);


const char *
get(const uint32_t id);


} // ns
} // ns


#endif // inc guard


#ifdef LIB_STRING_POOL_IMPL

#ifndef STRING_POOL_IMPL_INCLUDED_8B50BA25_E5FB_4CC4_951D_C5F7A28EA1D0
#define STRING_POOL_IMPL_INCLUDED_8B50BA25_E5FB_4CC4_951D_C5F7A28EA1D0


#include "array.hpp"
#include <string.h>
#include <stdlib.h>


// ------------------------------------------------- [ String Pool Interface] --


namespace {


lib::array<char*> str_pool;


} // anon ns



namespace LIB_NS_NAME {
namespace string_pool {


void
reset()
{
  for(size_t i = 0; i < str_pool.size(); ++i)
  {
    LIB_STRING_POOL_FREE(str_pool[i]);
  }
  
  str_pool.clear();
}


size_t
size()
{
  return str_pool.size();
}


uint32_t
add(const char *name)
{
  for(size_t i = 0; i < str_pool.size(); ++i)
  {
    if(strcmp(str_pool[i], name) == 0)
    {
      return i + 1;
    }
  }
  
  const size_t count = strlen(name);
  char *new_str = LIB_STRING_POOL_MALLOC(char, count);
  memcpy(new_str, name, count * sizeof(char));
  
  str_pool.emplace_back(new_str);
  return str_pool.size();
}


uint32_t
find(const char *name)
{
  for(size_t i = 0; i < str_pool.size(); ++i)
  {
    if(strcmp(name, str_pool[i]) == 0)
    {
      return i + 1;
    }
  }
  
  return 0;
}


const char *
get(const uint32_t id)
{
  if(id <= str_pool.size() && id > 0)
  {
    return str_pool[id - 1];
  }
  
  return "";
}


} // ns
} // ns


#endif // impl guard
#endif // include impl


