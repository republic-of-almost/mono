#include <roa_graph/roa_graph.h>
#include <catch/catch.hpp>


TEST_CASE("Local transforms")
{
  roa_graph_ctx_t graph_ctx = ROA_NULL;
  roa_graph_ctx_create(&graph_ctx);

  SECTION("Scale")
  {
    
  }

  roa_graph_ctx_destroy(&graph_ctx);
}