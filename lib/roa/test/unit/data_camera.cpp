#include <catch/catch.hpp>
#include <roa/camera.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA::Camera")
{
  lib::logging::set_output(0);
  
  
  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  SECTION("Create Defaults")
  {
    ROA::Camera data;
    
    REQUIRE(data.is_valid() == true);
    REQUIRE(data == true);
    REQUIRE(data.is_ref() == false);
  }
  
  
  SECTION("Create Null")
  {
    ROA::Camera data(nullptr);
    
    REQUIRE(data.is_valid() == false);
    REQUIRE(data == false);
    REQUIRE(data.is_ref() == false);
  }
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  
}
