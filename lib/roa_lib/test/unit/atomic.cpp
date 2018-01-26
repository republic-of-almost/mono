#include <roa_lib/atomic.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Atomic")
{
	roa_atomic_int at_int;
	roa_atomic_int_store(&at_int, 0);

  SECTION("Init")
  {
  	int i = roa_atomic_int_load(&at_int);

  	REQUIRE(i == 0);
  }

  SECTION("Store")
  {
  	printf("store\n");
  	int new_val = 123;
  	roa_atomic_int_store(&at_int, new_val);

  	int i = roa_atomic_int_load(&at_int);

  	printf("stored %d %d\n", new_val, i);
  	REQUIRE(new_val == i);
  }

  SECTION("Increment")
  {
  	int i = roa_atomic_int_inc(&at_int);

  	REQUIRE(i == 1);
  }
}