#include <roa_lib/array.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Array")
{
  SECTION("create with capacity")
  {
    int *i = nullptr;
    roa_array_create_with_capacity(i, 32);

    REQUIRE(roa_array_size(i) == 0);
    REQUIRE(roa_array_capacity(i) == 32);

    roa_array_destroy(i);
    REQUIRE(i == nullptr);
  }

  SECTION("create no capacity")
  {
    int *i = nullptr;
    roa_array_create(i);

    REQUIRE(roa_array_size(i) == 0);
    REQUIRE(roa_array_capacity(i) == 1);

    /* push should create capacity */
    roa_array_push(i, 123);

    REQUIRE(i[0] == 123);
    REQUIRE(roa_array_size(i) == 1);
    REQUIRE(roa_array_capacity(i) >= 1);

    roa_array_destroy(i);
    REQUIRE(i == nullptr);
  }

  SECTION("resize")
  {
    int *i = nullptr;
    roa_array_create_with_capacity(i, 16);

    roa_array_resize(i, 32);

    REQUIRE(roa_array_size(i) == 32);
    REQUIRE(roa_array_capacity(i) == 32);
  }  

  SECTION("push")
  {
    int *i = nullptr;
    roa_array_create(i);

    roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_push(i, 3);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 2);
    REQUIRE(i[2] == 3);

    REQUIRE(roa_array_size(i) == 3);
    REQUIRE(roa_array_capacity(i) == 4);
  }

  SECTION("pop")
  {
    int *i = nullptr;
    roa_array_create(i);

    roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_pop(i);
    roa_array_push(i, 3);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 3);

    REQUIRE(roa_array_size(i) == 2);
    REQUIRE(roa_array_capacity(i) == 2);
  }  

  SECTION("back")
  {
    int *i = nullptr;
    roa_array_create(i);

    roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_push(i, 3);

    REQUIRE(roa_array_back(i) == 3);
  }

  SECTION("erase")
  {
    int *i = nullptr;
    roa_array_create(i);

  	roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_push(i, 3);

    roa_array_erase(i, 1);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 3);

    REQUIRE(roa_array_size(i) == 2);
    REQUIRE(roa_array_capacity(i) == 4);
  }

  SECTION("insert middle")
  {
    int *i = nullptr;
    roa_array_create(i);

    roa_array_push(i, 1);
    roa_array_push(i, 3);

    roa_array_insert(i, 1, 2);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 2);
    REQUIRE(i[2] == 3);

    REQUIRE(roa_array_size(i) == 3);
    REQUIRE(roa_array_capacity(i) == 4);
  }

  SECTION("insert front")
  {
    int *i = nullptr;
    roa_array_create(i);

    roa_array_insert(i, 0, 3);
    roa_array_insert(i, 0, 2);
    roa_array_insert(i, 0, 1);

    REQUIRE(i[0] == 1);
    REQUIRE(i[1] == 2);
    REQUIRE(i[2] == 3);

    REQUIRE(roa_array_size(i) == 3);
    REQUIRE(roa_array_capacity(i) == 4);
  }

  SECTION("clear")
  {
    int *i = nullptr;
    roa_array_create(i);

    roa_array_push(i, 1);
    roa_array_push(i, 2);
    roa_array_push(i, 3);

    REQUIRE(roa_array_size(i) == 3);
    REQUIRE(roa_array_capacity(i) == 4);

    roa_array_clear(i);

    REQUIRE(roa_array_size(i) == 0);
    REQUIRE(roa_array_capacity(i) == 4);
  }
}