/*
  types
  --
  Some type helpers

  Copyright: public-domain 2017 - http://unlicense.org/
*/
#ifndef TYPES_INCLUDED_5FD988D5_036E_4258_B281_177280BCEF26
#define TYPES_INCLUDED_5FD988D5_036E_4258_B281_177280BCEF26


// ----------------------------------------------------------- [ Threading ] --


// This is slightly pedentic I know
#ifdef __APPLE__
#include <stdatomic.h>
#endif
#else
#include <atomic>
using std::atomic_bool;


#endif // inc guard
