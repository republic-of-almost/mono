#ifndef OBJECT_INCLUDED_07686CBB_D179_47A5_9E8C_5F0E451925A5
#define OBJECT_INCLUDED_07686CBB_D179_47A5_9E8C_5F0E451925A5


#include <stdint.h>


namespace AS_utils {


struct Object
{
  uint32_t id;
  uint32_t ref;
  
  void add_ref();
  void remove_ref();
};


template<class A, class B>
B* object_ref_cast(A* a)
{
  return reinterpret_cast<B*>(a);
}


/* utils */

void      object_ctor(void *mem);
void      object_dtor(void *mem);

Object    *object_new();

/* method */



} // ns


#endif // inc guard
