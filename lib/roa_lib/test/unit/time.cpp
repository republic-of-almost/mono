#include <roa_lib/time.h>
#include <roa_lib/fundamental.h>
#include <catch/catch.hpp>


/* ------------------------------------------------------- [ Test Config ] -- */


/* longer timer might be a better test, shorter better for CI */
#define ROALIB_TEST_LONG_TIMER 0

/* output data */
#define ROALIB_TEST_OUTPUT_SANITY 0


#ifndef ROALIB_TEST_OUTPUT_SANITY
#include <stdio.h>
#endif


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Time")
{
  SECTION("Get Time And Sleep")
  {
  	long sleep_time = 4;

  	if(ROA_IS_ENABLED(ROALIB_TEST_LONG_TIMER))
  	{
  		sleep_time = 2048;
  	}

  	unsigned long ms1 = roa_time_get_current_ms();
  	roa_sleep_ms(sleep_time);
  	unsigned long ms2 = roa_time_get_current_ms();

		unsigned long delta = ms2 - ms1;

  	if(ROA_IS_ENABLED(ROALIB_TEST_OUTPUT_SANITY))
  	{
  		printf("Start: %lu End: %lu Delta: %lu\n", ms1, ms2, delta);
  	}

  	long error_upper = sleep_time + 4;
  	long error_lower = sleep_time - 4;

  	REQUIRE(delta < error_upper);
  	REQUIRE(delta > error_lower);
  }
}


/* ------------------------------------------------------- [ Test Config ] -- */


#undef ROALIB_TEST_LONG_TIMER
#undef ROALIB_TEST_OUTPUT_SANITY