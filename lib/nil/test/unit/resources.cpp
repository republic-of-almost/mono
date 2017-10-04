#include <catch/catch.hpp>
#include <nil/nil.hpp>
#include <nil/resource/resource.hpp>


TEST_CASE("Resources")
{
  Nil_ctx *ctx;
  nil_ctx_initialize(&ctx);
  
  SECTION("Materials")
  {
    // load
    {
      Nil_material mat{};
      mat.color = 0xFF0000FF;
      mat.name = "Foo";
      
      nil_rsrc_material_create(ctx, &mat);
      
      REQUIRE(mat.id > 0);
      REQUIRE(mat.color == 0xFF0000FF);
    }
    
    // Find
    {
      Nil_material mat{};
      nil_rsrc_material_find_by_name(ctx, "Foo", &mat);
      
      REQUIRE(mat.color == 0xFF0000FF);
    }
    
    // Get
    {
      size_t count = 0;
      Nil_material *mats = nullptr;
      
      nil_rsrc_material_get_data(ctx, &count, &mats);
      
      REQUIRE(count == 2);
      REQUIRE(mats[1].color == 0xFF0000FF);
    }
  }
  
  nil_ctx_destroy(&ctx);
}
