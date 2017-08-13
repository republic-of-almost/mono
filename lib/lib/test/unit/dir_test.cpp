#include <catch/catch.hpp>
#include <lib/directory.hpp>


TEST_CASE("Dir Test")
{
   REQUIRE(strlen(lib::dir::exe_path()) > 0);
}
