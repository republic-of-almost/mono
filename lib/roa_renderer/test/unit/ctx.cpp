#include <catch/catch.hpp>
#include <roa_renderer/roa_renderer.h>


TEST_CASE("Ctx Setup")
{
  roa_renderer_ctx_t ctx = ROA_NULL;
  roa_renderer_ctx_create(&ctx, ROA_NULL);

  REQUIRE(ctx != ROA_NULL);

  /* check we initialized in an expected state */
  REQUIRE(roa_renderer_mesh_renderable_count(ctx) == 0);
  REQUIRE(roa_renderer_mesh_resource_count(ctx) == 0);
  REQUIRE(roa_renderer_camera_count(ctx) == 0);
  REQUIRE(roa_renderer_light_count(ctx) ==  0);
  REQUIRE(roa_renderer_material_resource_count(ctx) == 0);

  roa_renderer_ctx_destroy(&ctx);
  
  REQUIRE(ctx == ROA_NULL);
}