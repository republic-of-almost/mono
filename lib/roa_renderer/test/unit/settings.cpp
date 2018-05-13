#include <catch/catch.hpp>
#include <roa_renderer/roa_renderer.h>


TEST_CASE("Settings")
{
  roa_renderer_ctx_t ctx = ROA_NULL;
  roa_renderer_ctx_create(&ctx, ROA_NULL);

  SECTION("Viewport")
  {
    roa_renderer_device_viewport vp{};
    vp.width = 800;
    vp.height = 600;

    roa_renderer_set_device_viewport(ctx, &vp);
    
    roa_renderer_device_viewport get_vp{};
    roa_renderer_get_device_viewport(ctx, &get_vp);

    REQUIRE(get_vp.width == vp.width);
    REQUIRE(get_vp.height == vp.height);
  }

  roa_renderer_ctx_destroy(&ctx);
}