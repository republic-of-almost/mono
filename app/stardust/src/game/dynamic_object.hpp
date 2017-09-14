#ifndef DYNAMIC_OBJECTS_INCLUDED_9C12AB16_BFD8_4DA3_9F2E_CDD176ADBE80
#define DYNAMIC_OBJECTS_INCLUDED_9C12AB16_BFD8_4DA3_9F2E_CDD176ADBE80


#include <roa/roa.hpp>
#include <lib/timer.hpp>


namespace Dynamic_object {


struct Data
{
  ROA::Object entity = ROA::Object(nullptr);
  
  lib::milliseconds start = 0.f;
  lib::milliseconds end = 0.f;
  lib::milliseconds time = 0.f;
};


Data *
get_dyn_obj();


void
setup(Data *obj);


void
message_handler(ROA::Object obj, uint32_t id, uintptr_t data);


void
update(ROA::Object obj);


} // ns


#endif // inc guard