#include <roa_lib/alloc.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Alloc")
{
  SECTION("Basic Alloc")
  {
  	/* a better test is possible :) */
  	
  	void *bytes = roa_alloc(128);
  	roa_free(bytes);

  	REQUIRE(true);
  }
}