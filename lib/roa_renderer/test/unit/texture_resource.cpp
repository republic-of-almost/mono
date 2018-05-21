#include <catch/catch.hpp>
#include <roa_renderer/roa_renderer.h>


TEST_CASE("Texture Resource")
{
  roa_renderer_ctx_t ctx = ROA_NULL;
  roa_renderer_ctx_create(&ctx, ROA_NULL);

  REQUIRE(ctx != ROA_NULL);

  SECTION("Add Texture")
  {
    REQUIRE(roa_renderer_texture_resource_count(ctx) == 0);
    REQUIRE(roa_renderer_texture_resource_exists(ctx, "FOO") == ROA_FALSE);

    struct roa_renderer_texture_resource desc {};
    desc.name = "Some Name";
    
    roa_renderer_texture_resource_add(ctx, &desc);

    REQUIRE(roa_renderer_texture_resource_count(ctx) == 1);
  }

  roa_renderer_ctx_destroy(&ctx);
}