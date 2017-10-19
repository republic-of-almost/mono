#include <catch/catch.hpp>
#include <node/node.h>

TEST_CASE("Node - Node Attributes")
{
  Node_ctx *ctx = NULL;
  node_ctx_create(&ctx, NULL);
  
  SECTION("Name")
  {
  }
  
  SECTION("Relationships")
  {
  }
  
  SECTION("Bitfield")
  {
  }
  
  node_ctx_destroy(&ctx);
}
