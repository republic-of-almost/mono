#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/resource/shader.hpp>
#include <lib/logging.hpp>


TEST_CASE("Resource - Shader")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);
  
  SECTION("Initial")
  {
    const size_t count = nil_rsrc_shader_get_count(ctx);
  
    REQUIRE(count == 1);
  }
  
  SECTION("Load Pass")
  {
    Nil_shader shd{};
    shd.name = "shd_to_pass";
    
    shd.vs_code = "VS";
    shd.gs_code = "GS";
    shd.fs_code = "FS";
    
    const bool loaded  = !!nil_rsrc_shader_create(ctx, &shd);
    const size_t count = nil_rsrc_shader_get_count(ctx);
    
    REQUIRE(loaded == true);
    REQUIRE(count == 2);
    REQUIRE(shd.id > 0);
    
    Nil_shader get_shd{};
    
    nil_rsrc_shader_find_by_name(ctx, "shd_to_pass", &get_shd);
    
    REQUIRE(strcmp(shd.vs_code, get_shd.vs_code) == 0);
    REQUIRE(strcmp(shd.gs_code, get_shd.gs_code) == 0);
    REQUIRE(strcmp(shd.fs_code, get_shd.fs_code) == 0);
  }
  
  SECTION("Load Fail")
  {
    Nil_shader shd{};
    shd.name = "shd_to_fail";
    
    const bool should_pass = !!nil_rsrc_shader_create(ctx, &shd);
    const bool should_fail = !!nil_rsrc_shader_create(ctx, &shd);
    
    REQUIRE(should_pass == true);
    REQUIRE(should_fail == false);
  }
  
  nil_ctx_destroy(&ctx);
};
