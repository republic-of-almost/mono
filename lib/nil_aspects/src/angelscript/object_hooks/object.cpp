#include "object.hpp"
#include <new>


namespace AS_utils {


/* utils */


void
object_ctor(void *mem)
{
  new(mem) Object();
}


void
object_dtor(void *mem)
{
  ((Object*)mem)->~Object();
}


Object*
object_new()
{
  return new(Object);
}


} // ns
