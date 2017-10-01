#ifndef ROA_ASPECT_INCLUDED_FEBAF5C0_076C_42E2_A285_70400D002CCD
#define ROA_ASPECT_INCLUDED_FEBAF5C0_076C_42E2_A285_70400D002CCD


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil_ext {
namespace ROA_Aspect {


/* ---------------------------------------------------------- [ ROA Data ] -- */


struct Data
{
  
};


/* ------------------------------------------------------ [ ROA Lifetime ] -- */


void
start_up(Nil_ctx *ctx, void *data);


void
events(Nil_ctx *ctx, void *data);


void
shut_down(Nil_ctx *ctx, void *data);


/* --------------------------------------------------------- [ ROA Tasks ] -- */


void
logic_start_callback(Nil_ctx *ctx, void *data);


void
logic_update_callback(Nil_ctx *ctx, void *data);


} // ns
} // ns


#endif // inc guard
