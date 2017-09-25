#include <catch/catch.hpp>
#include <roa/transform.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA::Transform")
{
  lib::logging::set_output(0);
  
  
  // ----------------------------------------------------------- [ Lifetime ] --
  
  SECTION("Create Defaults")
  {
    ROA::Transform data;
    
    REQUIRE(data.is_valid() == true);
    REQUIRE(data == true);
    REQUIRE(data.is_ref() == false);
  }
  
  SECTION("Create Null")
  {
  }
  
  // ------------------------------------------------------------- [ Object ] --
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
}
