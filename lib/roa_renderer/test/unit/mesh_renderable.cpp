#include <catch/catch.hpp>
#include <roa_renderer/roa_renderer.h>


TEST_CASE("Mesh Renderer")
{
  roa_renderer_ctx_t ctx = ROA_NULL;
  roa_renderer_ctx_create(&ctx, ROA_NULL);

  REQUIRE(ctx != ROA_NULL);

  struct roa_renderer_mesh_renderable mesh_desc;
  ROA_MEM_ZERO(mesh_desc);

  mesh_desc.material_id = 1;
  mesh_desc.mesh_id = 2;
  mesh_desc.world_transform[0] = 3;
  mesh_desc.world_transform[1] = 4;
  mesh_desc.world_transform[2] = 5;
  mesh_desc.world_transform[3] = 6;
  mesh_desc.world_transform[4] = 7;
  mesh_desc.world_transform[5] = 8;
  mesh_desc.world_transform[6] = 9;
  mesh_desc.world_transform[7] = 10;
  mesh_desc.world_transform[8] = 11;
  mesh_desc.world_transform[9] = 12;
  mesh_desc.world_transform[10] = 13;
  mesh_desc.world_transform[11] = 14;
  mesh_desc.world_transform[12] = 15;
  mesh_desc.world_transform[13] = 16;
  mesh_desc.world_transform[14] = 17;
  mesh_desc.world_transform[15] = 18;

  REQUIRE(roa_renderer_camera_count(ctx) == 0);


  SECTION("Set/Get Mesh Renderable")
  {
    uint32_t rdr_id = 1;
    ROA_BOOL set_success = roa_renderer_mesh_renderable_set(ctx, &mesh_desc, rdr_id);
    REQUIRE(set_success == ROA_TRUE);

    REQUIRE(roa_renderer_mesh_renderable_count(ctx) == 1);

    struct roa_renderer_mesh_renderable get_desc {};
    ROA_BOOL get_success = roa_renderer_mesh_renderable_get(ctx, &get_desc, rdr_id);
    REQUIRE(get_success == ROA_TRUE);
    REQUIRE(get_desc.material_id == get_desc.material_id);
    REQUIRE(get_desc.mesh_id == get_desc.mesh_id);
    REQUIRE(get_desc.world_transform[0] == mesh_desc.world_transform[0]);
    REQUIRE(get_desc.world_transform[1] == mesh_desc.world_transform[1]);
    REQUIRE(get_desc.world_transform[2] == mesh_desc.world_transform[2]);
    REQUIRE(get_desc.world_transform[3] == mesh_desc.world_transform[3]);
    REQUIRE(get_desc.world_transform[4] == mesh_desc.world_transform[4]);
    REQUIRE(get_desc.world_transform[5] == mesh_desc.world_transform[5]);
    REQUIRE(get_desc.world_transform[6] == mesh_desc.world_transform[6]);
    REQUIRE(get_desc.world_transform[7] == mesh_desc.world_transform[7]);
    REQUIRE(get_desc.world_transform[8] == mesh_desc.world_transform[8]);
    REQUIRE(get_desc.world_transform[9] == mesh_desc.world_transform[9]);
    REQUIRE(get_desc.world_transform[10] == mesh_desc.world_transform[10]);
    REQUIRE(get_desc.world_transform[11] == mesh_desc.world_transform[11]);
    REQUIRE(get_desc.world_transform[12] == mesh_desc.world_transform[12]);
    REQUIRE(get_desc.world_transform[13] == mesh_desc.world_transform[13]);
    REQUIRE(get_desc.world_transform[14] == mesh_desc.world_transform[14]);
    REQUIRE(get_desc.world_transform[15] == mesh_desc.world_transform[15]);
  }

  SECTION("Set Mesh Renderable twice")
  {
    uint32_t rdr_id = 1;

    /* first set */

    ROA_BOOL set_first = roa_renderer_mesh_renderable_set(ctx, &mesh_desc, rdr_id);
    REQUIRE(set_first == ROA_TRUE);

    unsigned first_count = roa_renderer_mesh_renderable_count(ctx);
    REQUIRE(first_count == 1);

    /* set again */

    ROA_BOOL set_second = roa_renderer_mesh_renderable_set(ctx, &mesh_desc, rdr_id);
    REQUIRE(set_second == ROA_TRUE);

    unsigned second_count = roa_renderer_mesh_renderable_count(ctx);
    REQUIRE(second_count == 1);
  }

  SECTION("Has Mesh Renderable")
  {
    uint32_t rdr_id = 1;

    ROA_BOOL exists = roa_renderer_mesh_renderable_exists(ctx, rdr_id);
    REQUIRE(exists == ROA_FALSE);

    ROA_BOOL set = roa_renderer_mesh_renderable_set(ctx, &mesh_desc, rdr_id);
    REQUIRE(set == ROA_TRUE);

    ROA_BOOL second_exists = roa_renderer_mesh_renderable_exists(ctx, rdr_id);
    REQUIRE(second_exists == ROA_TRUE);
  }

  SECTION("Get Unknown Mesh Renderable")
  {
    uint32_t rdr_id = 1;
    struct roa_renderer_mesh_renderable get_desc {};
    ROA_BOOL get_success = roa_renderer_mesh_renderable_get(ctx, &get_desc, rdr_id);

    REQUIRE(get_success == ROA_FALSE);
  }

  SECTION("Set/Clear/Get Mesh Renderable")
  {
    uint32_t rdr_id = 1;

    ROA_BOOL set = roa_renderer_mesh_renderable_set(ctx, &mesh_desc, rdr_id);
    REQUIRE(set == ROA_TRUE);

    ROA_BOOL clear = roa_renderer_mesh_renderable_clear(ctx, rdr_id);
    REQUIRE(clear == ROA_TRUE);

    unsigned count = roa_renderer_mesh_renderable_count(ctx);
    REQUIRE(count == 0);

    ROA_BOOL exists = roa_renderer_mesh_renderable_exists(ctx, rdr_id);
    REQUIRE(exists == ROA_FALSE);
  }

  SECTION("Clear Unknown Mesh Renderable")
  {
    uint32_t rdr_id = 1;
    ROA_BOOL get_success = roa_renderer_mesh_renderable_clear(ctx, rdr_id);

    REQUIRE(get_success == ROA_FALSE);
  }
}