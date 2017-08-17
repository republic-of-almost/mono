#ifndef ROA_ASPECT_INCLUDED_FEBAF5C0_076C_42E2_A285_70400D002CCD
#define ROA_ASPECT_INCLUDED_FEBAF5C0_076C_42E2_A285_70400D002CCD


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil_ext {
namespace ROA_Aspect {


// ------------------------------------------------------ [ ROA Aspect Data ] --


struct Data
{
  
};


// ------------------------------------------------- [ ROA Aspect Interface ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect);


void
events(Nil::Engine &engine, Nil::Aspect &aspect);


// ------------------------------------------------------------ [ ROA Tasks ] --


void
logic_start_callback(Nil::Engine &engine, uintptr_t user_data);


void
logic_update_callback(Nil::Engine &engine, uintptr_t user_data);


} // ns
} // ns


#endif // inc guard
