#ifndef APPLICATION_INCLUDED_410EDE8C_F4B7_498C_82F8_676D317169C4
#define APPLICATION_INCLUDED_410EDE8C_F4B7_498C_82F8_676D317169C4


#include <roa/fundamental.hpp>


/* ---------------------------------------------- [ Application Lifetime ] -- */


typedef void(*roa_custom_tick_fn)(uintptr_t user_data);


void      roa_application_init();
void      roa_application_destroy();

void      roa_application_run();
void      roa_application_run_with_tick(uintptr_t user_data, roa_custom_tick_fn cb);


#endif // inc guard
