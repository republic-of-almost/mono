#include <catch/catch.hpp>
#include <lib/directory.hpp>
#include <lib/string.hpp>


TEST_CASE("Dir Test")
{
  // Without binding this to a machine I don't know how I can test this.
  REQUIRE(lib::str::length(lib::dir::exe_path()) > 0);
}
