#include <catch/catch.hpp>
#include <lib/utilities.hpp>


TEST_CASE("Key")
{
  SECTION("Linear Search")
  {
    const uint32_t keys[] {1,2,3,4,5,6,7,8,9,10};
    size_t index = 0;

    const bool found_1 = lib::key::linear_search(1, keys, sizeof(keys) / sizeof(uint32_t), &index);

    REQUIRE(found_1);
    REQUIRE(index == 0);

    const bool found_10 = lib::key::linear_search(10, keys, sizeof(keys) / sizeof(uint32_t), &index);

    REQUIRE(found_10);
    REQUIRE(index == 9);

    const bool found_11 = lib::key::linear_search(11, keys, sizeof(keys) / sizeof(uint32_t), &index);

    REQUIRE(!found_11);
  }
}
