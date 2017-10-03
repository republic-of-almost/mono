#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/resource/texture.hpp>
#include <lib/logging.hpp>


TEST_CASE("Resource - Texture")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);
  
  SECTION("Initial")
  {
    const size_t expected_size = 1;
    const size_t size = nil_rsrc_texture_get_count(ctx);
    
    REQUIRE(expected_size == size);
  }
  
  SECTION("Load Pass")
  {
    Nil_texture data{};
    data.name = "texture_to_pass";
    
    const char *filename = "some_filename";
    data.data = (uintptr_t)filename;
    data.data_size = strlen(filename) + 1;
    data.data_type = NIL_DATA_FILENAME;
    
    const bool loaded = !!nil_rsrc_texture_create(ctx, &data);
    
    const size_t count = nil_rsrc_texture_get_count(ctx);
    const size_t expected_count = 2;
    
    REQUIRE(loaded == true);
    REQUIRE(count == expected_count);
    REQUIRE(data.id > 0);
  }
  
  SECTION("Load Fail")
  {
    Nil_texture data{};
    data.name = "texture_to_fail";
    
    const char *filename = "some_other_filename";
    data.data = (uintptr_t)filename;
    data.data_size = strlen(filename) + 1;
    data.data_type = NIL_DATA_FILENAME;
    
    const bool should_pass = !!nil_rsrc_texture_create(ctx, &data);
    const bool should_fail = !!nil_rsrc_texture_create(ctx, &data);
    
    REQUIRE(should_pass == true);
    REQUIRE(should_fail == false);
  }
  
  nil_ctx_destroy(&ctx);
}
