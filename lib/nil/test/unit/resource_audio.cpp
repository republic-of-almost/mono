#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/resource/audio.hpp>
#include <lib/logging.hpp>


TEST_CASE("Resource - Audio")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);
  
  SECTION("Initial")
  {
    const size_t expected_size = 1;
    const size_t actual_size = nil_rsrc_audio_src_get_count(ctx);
  
    REQUIRE(expected_size == actual_size);
  }
  
  SECTION("Load Pass")
  {
    Nil_audio_src data{};
    data.name = "audio_to_pass";
    
    const char *filename = "some_filename";
    data.data = (uintptr_t)filename;
    data.data_size = strlen(filename) + 1;
    data.data_type = NIL_DATA_FILENAME;
    
    const bool loaded = !!nil_rsrc_audio_src_create(ctx, &data);
    
    const size_t expected_size = 2;
    const size_t actual_size = nil_rsrc_audio_src_get_count(ctx);

    REQUIRE(loaded == true);
    REQUIRE(expected_size == actual_size);
    REQUIRE(data.id > 0);
  }
  
  SECTION("Load Fail")
  {
    Nil_audio_src data{};
    data.name = "audio_to_fail";
    
    const char *filename = "some_other_filename";
    data.data = (uintptr_t)filename;
    data.data_size = strlen(filename) + 1;
    data.data_type = NIL_DATA_FILENAME;
    
    const bool should_pass = !!nil_rsrc_audio_src_create(ctx, &data);
    const bool should_fail = !!nil_rsrc_audio_src_create(ctx, &data);
    
    REQUIRE(should_pass == true);
    REQUIRE(should_fail == false);
  }
  
  nil_ctx_destroy(&ctx);
}
