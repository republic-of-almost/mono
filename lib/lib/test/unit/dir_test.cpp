#include <catch/catch.hpp>
#include <lib/directory.hpp>
#include <lib/string.hpp>


TEST_CASE("Dir Test")
{
   REQUIRE(strlen(lib::dir::exe_path()) > 0);
}
