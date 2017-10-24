#include <catch/catch.hpp>
#include <codex/codex.h>


TEST_CASE("Codex Properties")
{
  Codex_ctx *ctx;
  codex_create(&ctx);
  
  SECTION("Add Properties")
  {
    const uint32_t id = codex_object_type_create(ctx, "foo");
    REQUIRE(id > 0);

    const uint32_t obj_id   = codex_object_type_create(ctx, "bar");
    const uint32_t prop1_id = codex_property_create(ctx, obj_id, CODEX_PROP_INT, "integer");
    
    REQUIRE(prop1_id > 0);
    
    const uint32_t prop2_id = codex_property_create(ctx, obj_id, CODEX_PROP_FLOAT, "float");
    
    REQUIRE(prop2_id > 0);
  }
  
  SECTION("Add Duplicate Properties")
  {
    const uint32_t id       = codex_object_type_create(ctx, "foo");
    const uint32_t obj_id   = codex_object_type_create(ctx, "bar");
    const uint32_t prop1_id = codex_property_create(ctx, obj_id, CODEX_PROP_INT, "integer");
    
    REQUIRE(prop1_id > 0);
    
    const uint32_t prop2_id = codex_property_create(ctx, obj_id, CODEX_PROP_FLOAT, "integer");
    
    REQUIRE(prop2_id == 0);
  }
  
  SECTION("Add Property data")
  {
    const uint32_t id       = codex_object_type_create(ctx, "foo");
    const uint32_t obj_id   = codex_object_type_create(ctx, "bar");
    const uint32_t prop1_id = codex_property_create(ctx, obj_id, CODEX_PROP_INT, "integer");
    const uint32_t inst_id  = codex_instance_create(ctx);
    
    codex_instance_add_object(ctx, inst_id, obj_id);
    
    REQUIRE(false); // adding property data
  }
  
  codex_destroy(&ctx);
}
