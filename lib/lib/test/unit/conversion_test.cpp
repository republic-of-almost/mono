#include <catch/catch.hpp>
#include <lib/convert.hpp>


TEST_CASE("Conversions")
{
  SECTION("MB to Bytes")
  {
    REQUIRE(lib::convert::mb_to_bytes(1) == 1048576);
    REQUIRE(lib::convert::mb_to_bytes(2) == 1048576 * 2);
  }
}
