#include <catch/catch.hpp>
#include <rov/rov.hpp>
#include <lib/logging.hpp>

TEST_CASE("ROV - Resources")
{
  rov_initialize(rovGraphicsApi_NOP, "./");
  
  SECTION("Create Texture")
  {
    REQUIRE(true);
  }
  
  rov_destroy();
}
