#include <catch/catch.hpp>
#include <codex/codex.h>


TEST_CASE("Codex Properties")
{
  Codex_ctx *ctx;
  codex_create(&ctx);
  
  SECTION("Add Properties")
  {
    const uint32_t obj_id = codex_object_type_create(ctx, "LOCAL_TRANSFORM");
    
    const uint32_t prop1_id = codex_property_create(
      ctx,
      obj_id,
      CODEX_PROP_INT,
      "SCALE"
    );
    
    REQUIRE(prop1_id > 0);
    
    const uint32_t prop2_id = codex_property_create(
      ctx,
      obj_id,
      CODEX_PROP_FLOAT,
      "ROTATION"
    );
    REQUIRE(prop2_id > 0);
  }
  
  SECTION("Add Duplicate Properties")
  {
    const uint32_t obj_id = codex_object_type_create(
      ctx,
      "LOCAL_TRANSFORM"
    );
    
    const uint32_t prop1_id = codex_property_create(
      ctx,
      obj_id,
      CODEX_PROP_INT,
      "POSITION"
    );
    REQUIRE(prop1_id > 0);
    
    const uint32_t prop2_id = codex_property_create(
      ctx,
      obj_id,
      CODEX_PROP_FLOAT,
      "POSITION"
    );
    REQUIRE(prop2_id == 0);
  }
  
  SECTION("Add Property data")
  {
    const uint32_t obj_id   = codex_object_type_create(ctx, "LOCAL_TRANSFORM");
    const uint32_t prop1_id = codex_property_create(ctx, obj_id, CODEX_PROP_INT, "POSITION");
    const uint32_t inst_id  = codex_instance_create(ctx);

    const Codex_bool obj_should_not = codex_instance_has_object(ctx, inst_id, obj_id);
    REQUIRE(obj_should_not == CODEX_FALSE);
    
    const Codex_bool obj_added = codex_instance_add_object(ctx, inst_id, obj_id);
    REQUIRE(obj_added == CODEX_TRUE);
    
    const Codex_bool obj_exists = codex_instance_has_object(ctx, inst_id, obj_id);
    REQUIRE(obj_exists == CODEX_TRUE);
  }
  
  SECTION("Set data")
  {
    const uint32_t obj_id   = codex_object_type_create(ctx, "LOCAL_TRANSFORM");
    const uint32_t prop1_id = codex_property_create(ctx, obj_id, CODEX_PROP_INT, "POSITION");
    const uint32_t inst_id  = codex_instance_create(ctx);
    
    codex_instance_add_object(ctx, inst_id, obj_id);
    
    codex_property_set_float();
  }
  
  codex_destroy(&ctx);
}
