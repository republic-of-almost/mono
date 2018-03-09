#include <roa_lib/atomic.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Atomic")
{
  constexpr int init_value = 5;

	roa_atomic_int at_int;
	roa_atomic_int_store(&at_int, init_value);

  SECTION("Init")
  {
  	int i = roa_atomic_int_load(&at_int);

  	REQUIRE(i == init_value);
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
    int j = roa_atomic_int_load(&at_int);

  	REQUIRE(i == init_value + 1);
    REQUIRE(j == init_value + 1);
  }

  SECTION("Decrement")
  {
    int i = roa_atomic_int_dec(&at_int);
    int j = roa_atomic_int_load(&at_int);

    REQUIRE(i == init_value);
    REQUIRE(j == init_value - 1);
  }

  SECTION("Addition")
  {
    constexpr int add = 123;

    int i = roa_atomic_int_add(&at_int, add);
    int j = roa_atomic_int_load(&at_int);

    REQUIRE(i == init_value);
    REQUIRE(j == init_value + add);
  }

  SECTION("Subtract")
  {
    constexpr int sub = -123;

    int i = roa_atomic_int_add(&at_int, sub);
    int j = roa_atomic_int_load(&at_int);

    REQUIRE(i == init_value);
    REQUIRE(j == init_value + sub);
  }

  SECTION("Swap")
  {
    constexpr int swap = 456;

    int i = roa_atomic_int_swap(&at_int, swap);
    int j = roa_atomic_int_load(&at_int);

    REQUIRE(i == init_value);
    REQUIRE(j == swap);
  }

  SECTION("Compare Swap Pass")
  {
    constexpr int swap = 456;

    int i = roa_atomic_int_compare_and_swap(&at_int, init_value, swap);
    int j = roa_atomic_int_load(&at_int);

    REQUIRE(i == init_value);
    REQUIRE(j == swap);
  }

  SECTION("Compare Swap Fail")
  {
    constexpr int swap = 456;

    int i = roa_atomic_int_compare_and_swap(&at_int, init_value + 1, swap);
    int j = roa_atomic_int_load(&at_int);

    REQUIRE(i == init_value);
    REQUIRE(j == init_value);
  }   
}