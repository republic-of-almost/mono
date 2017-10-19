#include <catch/catch.hpp>
#include <node/node.h>

TEST_CASE("Node - Context Lifetime")
{
  SECTION("Create / Destroy")
  {
    Node_ctx *ctx = NULL;
    REQUIRE(ctx == NULL);
  
    node_ctx_create(&ctx, NULL);
    REQUIRE(ctx != NULL);
    
    node_ctx_destroy(&ctx);
    REQUIRE(ctx == NULL);
  }
  
  SECTION("Create With Hints")
  {
    Node_ctx_hints hints{};
    hints.capacity_hint = 512;
  
    Node_ctx *ctx = NULL;
    REQUIRE(ctx == NULL);
  
    node_ctx_create(&ctx, &hints);
    REQUIRE(ctx != NULL);
    
    node_ctx_destroy(&ctx);
    REQUIRE(ctx == NULL);
  }
}
