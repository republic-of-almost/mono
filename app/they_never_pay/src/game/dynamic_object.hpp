#ifndef DYNAMIC_OBJECTS_INCLUDED_9C12AB16_BFD8_4DA3_9F2E_CDD176ADBE80
#define DYNAMIC_OBJECTS_INCLUDED_9C12AB16_BFD8_4DA3_9F2E_CDD176ADBE80


#include <roa/roa.hpp>


namespace Game {


struct Dynamic_object
{
  ROA::Object entity;
};


void
setup(Dynamic_object *obj);


} // ns


#endif // inc guard
