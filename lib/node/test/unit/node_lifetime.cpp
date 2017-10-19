#include <catch/catch.hpp>
#include <node/node.h>

TEST_CASE("Node - Node Lifetime")
{
  Node_ctx *ctx = NULL;
  node_ctx_create(&ctx, NULL);
  
  SECTION("Create / Destroy Node")
  {
  }
  
  node_ctx_destroy(&ctx);
}
