#include <catch/catch.hpp>
#include <codex/codex.h>


TEST_CASE("Codex Objects")
{
  Codex_ctx *ctx;
  codex_create(&ctx);
  
  SECTION("Create Objects")
  {
    const uint32_t id = codex_object_type_create(ctx, "foo");
    REQUIRE(id > 0);
    
    const size_t count = codex_object_type_count(ctx);
    REQUIRE(count == 1);
  }
  
  codex_destroy(&ctx);
}
