#include <catch/catch.hpp>
#include <roa/audio_player.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA::Audio_player")
{
  lib::logging::set_output(0);
  
  
  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  SECTION("Create Defaults")
  {
    ROA::Audio_player data;
    
    REQUIRE(data.is_valid() == true);
    REQUIRE(data == true);
    REQUIRE(data.is_ref() == false);
  }
  
  
  SECTION("Create Null")
  {
    ROA::Audio_player data(nullptr);
    
    REQUIRE(data.is_valid() == false);
    REQUIRE(data == false);
    REQUIRE(data.is_ref() == false);
  }
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  
}
