#include <catch/catch.hpp>
#include <roa/audio.hpp>
#include <lib/logging.hpp>


TEST_CASE("ROA Resource - Audio")
{
  lib::logging::set_output(0);

  ROA::Audio audio1{};
  REQUIRE(audio1.get_id() == 0);

  ROA::Audio audio2{"some/path.ogg"};
  REQUIRE(audio2.get_id() > 0);
  
  ROA::Audio audio3{audio2.get_id()};
  REQUIRE(audio2.get_id() == audio3.get_id());
}


