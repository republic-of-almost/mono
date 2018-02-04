#include <roa_lib/assert.h>
#include <roa_lib/fundamental.h>
#include <catch/catch.hpp>


/* ------------------------------------------------------- [ Test Config ] -- */


#define ROALIB_TEST_ASSERT 0
#define ROALIB_TEST_ASSERT_PEDANTIC 0


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Assert")
{
	ROA_ASSERT(true);
	ROA_ASSERT_PEDANTIC(true);

	/* hard to test in an auto test */
	/* so here it is for manual purpose */
	if(ROA_IS_ENABLED(ROALIB_TEST_ASSERT))
	{
		ROA_ASSERT(false);
	}

	if(ROA_IS_ENABLED(ROALIB_TEST_ASSERT_PEDANTIC))
	{
		ROA_ASSERT_PEDANTIC(false);
	}
}


/* ------------------------------------------------------- [ Test Config ] -- */


#undef ROALIB_TEST_ASSERT
#undef ROALIB_TEST_ASSERT_PEDANTIC
