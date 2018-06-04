#include <catch/catch.hpp>
#include <roa_renderer/roa_renderer.h>


TEST_CASE("Mesh Renderer")
{
  roa_renderer_ctx_t ctx = ROA_NULL;
  roa_renderer_ctx_create(&ctx, ROA_NULL);

  REQUIRE(ctx != ROA_NULL);
  REQUIRE(roa_renderer_camera_count(ctx) == 0);

  SECTION("Set/Get Mesh Renderable")
  {
  
  }

  SECTION("Set Mesh Renderable twice")
  {
    uint32_t rdr_id = 1;

    /* first set */

    ROA_BOOL set_first = roa_renderer_mesh_renderable_create(ctx, rdr_id);
    REQUIRE(set_first == ROA_TRUE);

    unsigned first_count = roa_renderer_mesh_renderable_count(ctx);
    REQUIRE(first_count == 1);

    /* set again */

    ROA_BOOL set_second = roa_renderer_mesh_renderable_create(ctx, rdr_id);
    REQUIRE(set_second == ROA_FALSE);

    unsigned second_count = roa_renderer_mesh_renderable_count(ctx);
    REQUIRE(second_count == 1);
  }

  SECTION("Has Mesh Renderable")
  {
    uint32_t rdr_id = 1;

    ROA_BOOL exists = roa_renderer_mesh_renderable_exists(ctx, rdr_id);
    REQUIRE(exists == ROA_FALSE);

    ROA_BOOL set = roa_renderer_mesh_renderable_create(ctx, rdr_id);
    REQUIRE(set == ROA_TRUE);

    ROA_BOOL second_exists = roa_renderer_mesh_renderable_exists(ctx, rdr_id);
    REQUIRE(second_exists == ROA_TRUE);
  }

  SECTION("Get Unknown Mesh Renderable")
  {
//    uint32_t rdr_id = 1;
//    struct roa_renderer_mesh_renderable get_desc {};
//    ROA_BOOL get_success = roa_renderer_mesh_renderable_get(ctx, &get_desc, rdr_id);
//
//    REQUIRE(get_success == ROA_FALSE);
  }

  SECTION("Set/Clear/Get Mesh Renderable")
  {
    uint32_t rdr_id = 1;

    ROA_BOOL set = roa_renderer_mesh_renderable_create(ctx, rdr_id);
    REQUIRE(set == ROA_TRUE);

    ROA_BOOL clear = roa_renderer_mesh_renderable_destroy(ctx, rdr_id);
    REQUIRE(clear == ROA_TRUE);

    unsigned count = roa_renderer_mesh_renderable_count(ctx);
    REQUIRE(count == 0);

    ROA_BOOL exists = roa_renderer_mesh_renderable_exists(ctx, rdr_id);
    REQUIRE(exists == ROA_FALSE);
  }

  SECTION("Clear Unknown Mesh Renderable")
  {
    uint32_t rdr_id = 1;
    ROA_BOOL get_success = roa_renderer_mesh_renderable_destroy(ctx, rdr_id);

    REQUIRE(get_success == ROA_FALSE);
  }
}
