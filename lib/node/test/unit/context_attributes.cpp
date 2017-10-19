#include <catch/catch.hpp>
#include <node/node.h>

TEST_CASE("Node - Context Attributes")
{
  Node_ctx *ctx = NULL;
  node_ctx_create(&ctx, NULL);
  
  SECTION("Set callbacks")
  {
    /* Can only make sure these functions work */
    node_ctx_info_log_cb(ctx, [](const char *msg) {});
    node_ctx_warn_log_cb(ctx, [](const char *msg) {});
    node_ctx_error_log_cb(ctx, [](const char *msg) {});
  }
  
  SECTION("Stats")
  {
    Node_ctx_stats stats;
    node_ctx_get_stats(ctx, &stats);
    
    REQUIRE(stats.count == 0);
  }
  
  node_ctx_destroy(&ctx);
}
