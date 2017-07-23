/*
  Assert
  --
  Anything todo with asserting.

  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef ASSERT_INCLUDED_764C490E_5ED6_4ACA_BE1C_723A2EBA0074
#define ASSERT_INCLUDED_764C490E_5ED6_4ACA_BE1C_723A2EBA0074


#include <assert.h>


/*
  Assert on a condition.
*/
#define LIB_ASSERT(value) assert(value)
#define LIB_ASSERT_MSG(value, msg) assert(value)


#endif // inc guard
