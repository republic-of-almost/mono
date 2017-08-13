#include <catch/catch.hpp>
#include <lib/timer.hpp>


TEST_CASE("Timer Test")
{
  // Hard to validiate this in a unit test.
  REQUIRE(lib::timer::get_current_time() != 0);
}
