#include <catch/catch.hpp>
#include <roa/renderable.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA::Renderable")
{
  lib::logging::set_output(0);
  
  
  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  SECTION("Create Defaults")
  {
    ROA::Renderable data;
    
    REQUIRE(data.is_valid() == true);
    REQUIRE(data == true);
    REQUIRE(data.is_ref() == false);
  }
  
  
  SECTION("Create Null")
  {
    ROA::Renderable data(nullptr);
    
    REQUIRE(data.is_valid() == false);
    REQUIRE(data == false);
    REQUIRE(data.is_ref() == false);
  }
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  
}
