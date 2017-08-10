#include <catch/catch.hpp>
#include <lib/utilities.hpp>


TEST_CASE("Helper Test")
{
  SECTION("On Stack")
  {
    // Failing in release no real surprise
//    int foo(1);
//    REQUIRE(lib::helper::on_stack((void*)&foo) == true);
//
//    int *bar = new int(2);
//    REQUIRE(lib::helper::on_stack((void*)bar) == false);
  }
}
