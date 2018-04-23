#include <catch/catch.hpp>
#include <roa_renderer/roa_renderer.h>


TEST_CASE("Camera")
{
  roa_renderer_ctx_t ctx = ROA_NULL;
  roa_renderer_ctx_create(&ctx, ROA_NULL);

  REQUIRE(ctx != ROA_NULL);

  struct roa_renderer_camera cam_desc {};
  cam_desc.near_plane     = 0.123f;
  cam_desc.far_plane      = 123.f;
  cam_desc.field_of_view  = 0.1f;
  cam_desc.position[0]    = 0.f;
  cam_desc.position[1]    = 0.f;
  cam_desc.position[2]    = 0.f;
  cam_desc.lookat[0]      = 0.f;
  cam_desc.lookat[1]      = 0.f;
  cam_desc.lookat[2]      = 1.f;
  cam_desc.up[0]          = 0.f;
  cam_desc.up[1]          = 1.f;
  cam_desc.up[2]          = 0.f;

  SECTION("Set/Get Camera")
  {
    uint32_t cam_id = 1;
    ROA_BOOL set_success = roa_renderer_camera_set(ctx, &cam_desc, cam_id);
    REQUIRE(set_success == ROA_TRUE);

    struct roa_renderer_camera get_desc {};
    ROA_BOOL get_success = roa_renderer_camera_get(ctx, &get_desc, cam_id);
    REQUIRE(get_success == ROA_TRUE);
    REQUIRE(cam_desc.near_plane == get_desc.near_plane);
    REQUIRE(cam_desc.far_plane == get_desc.far_plane);
    REQUIRE(cam_desc.field_of_view == get_desc.field_of_view);
    REQUIRE(cam_desc.position[0] == get_desc.position[0]);
    REQUIRE(cam_desc.position[1] == get_desc.position[1]);
    REQUIRE(cam_desc.position[2] == get_desc.position[2]);
    REQUIRE(cam_desc.lookat[0] == get_desc.lookat[0]);
    REQUIRE(cam_desc.lookat[1] == get_desc.lookat[1]);
    REQUIRE(cam_desc.lookat[2] == get_desc.lookat[2]);
    REQUIRE(cam_desc.up[0] == get_desc.up[0]);
    REQUIRE(cam_desc.up[1] == get_desc.up[1]);
    REQUIRE(cam_desc.up[2] == get_desc.up[2]);
  }

  SECTION("Set camera twice")
  {

  }

  SECTION("Has camera")
  {

  }

  SECTION("Get Unknown Camera")
  {
    uint32_t cam_id = 1;
    struct roa_renderer_camera get_desc {};
    ROA_BOOL get_success = roa_renderer_camera_get(ctx, &get_desc, cam_id);

    REQUIRE(get_success == ROA_FALSE);
  }

  SECTION("Set/Clear/Get Camera")
  {

  }

  SECTION("Clear Unknown Camera")
  {
    uint32_t cam_id = 1;
    ROA_BOOL get_success = roa_renderer_camera_clear(ctx, cam_id);

    REQUIRE(get_success == ROA_TRUE);
  }
}
