//#include <catch/catch.hpp>
//#include <roa/audio_source.hpp>
//#include <lib/logging.hpp>
//
//
//TEST_CASE("ROA Resource - Audio Source")
//{
//  lib::logging::set_output(0);
//
//  ROA::Audio_source audio1{};
//  REQUIRE(audio1.get_id() == 0);
//
//  ROA::Audio_source audio2{"some/path.ogg"};
//  REQUIRE(audio2.get_id() > 0);
//  
//  ROA::Audio_source audio3{audio2.get_id()};
//  REQUIRE(audio2.get_id() == audio3.get_id());
//}
