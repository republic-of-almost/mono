#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/resource/audio.hpp>
#include <lib/logging.hpp>


TEST_CASE("Resource - Audio")
{
  Nil::Engine nil_engine;
  lib::logging::set_output(0);
  
  SECTION("Initial")
  {
    REQUIRE(Nil::Resource::audio_count() == 1);
  }
  
  SECTION("Load Pass")
  {
    Nil::Resource::Audio data{};
    data.name = "audio_to_pass";
    
    const char *filename = "some_filename";
    data.data = (uintptr_t)filename;
    data.data_size = strlen(filename) + 1;
    data.data_type = Nil::Resource::Audio::FILENAME;
    
    const bool loaded = Nil::Resource::load(data);
    
    REQUIRE(loaded == true);
    REQUIRE(Nil::Resource::audio_count() == 2);
    REQUIRE(data.id > 0);
  }
  
  SECTION("Load Fail")
  {
    Nil::Resource::Audio data{};
    data.name = "audio_to_fail";
    
    const char *filename = "some_other_filename";
    data.data = (uintptr_t)filename;
    data.data_size = strlen(filename) + 1;
    data.data_type = Nil::Resource::Audio::FILENAME;
    
    const bool should_pass = Nil::Resource::load(data);
    const bool should_fail = Nil::Resource::load(data);
    
    REQUIRE(should_pass == true);
    REQUIRE(should_fail == false);
  }
}