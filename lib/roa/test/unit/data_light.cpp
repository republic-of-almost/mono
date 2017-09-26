#include <catch/catch.hpp>
#include <roa/light.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA::Light")
{
  lib::logging::set_output(0);
  
  
  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  SECTION("Create Defaults")
  {
    ROA::Light data;
    
    REQUIRE(data.is_valid() == true);
    REQUIRE(data == true);
    REQUIRE(data.is_ref() == false);
  }
  
  
  SECTION("Create Null")
  {
    ROA::Light data(nullptr);
    
    REQUIRE(data.is_valid() == false);
    REQUIRE(data == false);
    REQUIRE(data.is_ref() == false);
  }
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  
}
