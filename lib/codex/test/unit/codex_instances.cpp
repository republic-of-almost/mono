#include <catch/catch.hpp>
#include <codex/codex.h>


TEST_CASE("Codex Instances")
{
  Codex_ctx *ctx;
  codex_create(&ctx);
  
  SECTION("Create/Destroy Instance")
  {
    const uint32_t id = codex_instance_create(ctx);
    REQUIRE(id > 0);
    
    const size_t count = codex_instance_count(ctx);
    REQUIRE(count == 1);
    
    const bool removed = codex_instance_destroy(ctx, id);
    REQUIRE(removed == true);
    
    const size_t new_count = codex_instance_count(ctx);
    REQUIRE(new_count == 0);
  }
  
  codex_destroy(&ctx);
}
