#include <catch/catch.hpp>
#include <roa_renderer/roa_renderer.h>


TEST_CASE("Mesh Resource")
{
  roa_renderer_ctx_t ctx = ROA_NULL;
  roa_renderer_ctx_create(&ctx, ROA_NULL);

  REQUIRE(ctx != ROA_NULL);

  SECTION("Add Mesh")
  {
    REQUIRE(roa_renderer_mesh_resource_count(ctx) == 0);

    struct roa_renderer_mesh_resource desc {};
    desc.name = "Some Name";
    
    roa_renderer_mesh_resource_add(ctx, &desc);

    REQUIRE(roa_renderer_mesh_resource_count(ctx) == 1);
  }

  roa_renderer_ctx_destroy(&ctx);
}