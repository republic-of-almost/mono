#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/resource/mesh.hpp>
#include <lib/logging.hpp>


TEST_CASE("Resource - Mesh")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);
  
  SECTION("Initial")
  {
    const size_t count = nil_rsrc_mesh_get_count(ctx);
    const size_t expected_count = 1;
    
    REQUIRE(count == expected_count);
  }
  
  SECTION("Load Pass")
  {
    Nil_mesh mesh{};
    mesh.name = "mesh_to_pass";
    
    float some_data[] = {1,2,3};
    mesh.position_vec3 = some_data;
    mesh.triangle_count = 1;
    
    const uint32_t id = nil_rsrc_mesh_create(ctx, &mesh);
    
    REQUIRE(id > 0);
    
    const size_t count = nil_rsrc_mesh_get_count(ctx);
    const size_t expected_count = 2;
    
    REQUIRE(count == expected_count);
  }
  
  SECTION("Load Fail")
  {
    Nil_mesh mesh{};
    mesh.name = "mesh_to_fail";
    
    float some_data[] = {1,2,3};
    mesh.position_vec3 = some_data;
    mesh.triangle_count = 1;
    
    const uint32_t should_pass = !!nil_rsrc_mesh_create(ctx, &mesh);
    const uint32_t should_fail = !nil_rsrc_mesh_create(ctx, &mesh);
    
    REQUIRE(should_pass == true);
    REQUIRE(should_fail == true);
  }
  
  nil_ctx_destroy(&ctx);
};
