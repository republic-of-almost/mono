#include <catch/catch.hpp>
#include <lib/helpers.hpp>


TEST_CASE("Helper Test")
{
  SECTION("On Stack")
  {
    int foo(1);
    REQUIRE(lib::helper::on_stack((void*)&foo) == true);

    int *bar = new int(2);
    REQUIRE(lib::helper::on_stack((void*)bar) == false);
  }
}
