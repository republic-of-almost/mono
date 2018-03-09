#include <roa_lib/array.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Array")
{
  int *i = nullptr;
  roa_array_create(i, 32);

  SECTION("create")
  {
    REQUIRE(roa_array_size(i) == 0);
    REQUIRE(roa_array_capacity(i) == 32);

    roa_array_destroy(i);
    REQUIRE(i == nullptr);
  }

  SECTION("resize")
  {
    roa_array_resize(i, 32);

    REQUIRE(roa_array_size(i) == 32);
    REQUIRE(roa_array_capacity(i) == 32);
  }  

  SECTION("push")
  {
    roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_push(i, 3);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 2);
    REQUIRE(i[2] == 3);

    REQUIRE(roa_array_size(i) == 3);
    REQUIRE(roa_array_capacity(i) == 32);
  }

  SECTION("pop")
  {
    roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_pop(i);
    roa_array_push(i, 3);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 3);

    REQUIRE(roa_array_size(i) == 2);
    REQUIRE(roa_array_capacity(i) == 32);
  }  

  SECTION("back")
  {
    roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_push(i, 3);

    REQUIRE(roa_array_back(i) == 3);
  }

  SECTION("erase")
  {
  	roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_push(i, 3);

    roa_array_erase(i, 1);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 3);

    REQUIRE(roa_array_size(i) == 2);
    REQUIRE(roa_array_capacity(i) == 32);
  }

  SECTION("insert")
  {
    roa_array_push(i, 1);
    roa_array_push(i, 3);

    roa_array_insert(i, 1, 1);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 2);
    REQUIRE(i[2] == 3);

    REQUIRE(roa_array_size(i) == 3);
    REQUIRE(roa_array_capacity(i) == 32);
  }
}