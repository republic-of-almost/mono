#include <roa_graph/roa_graph.h>
#include <catch/catch.hpp>


TEST_CASE("Nodes")
{
  roa_graph_ctx_t graph_ctx = ROA_NULL;
  roa_graph_ctx_create(&graph_ctx);
  
  REQUIRE(graph_ctx);

  SECTION("Inital state")
  {
    unsigned count = roa_graph_node_count(graph_ctx);
    REQUIRE(count == 0);
  }

  SECTION("Add nodes")
  {
    uint32_t new_node = roa_graph_node_create(graph_ctx);
    REQUIRE(new_node > 0);

    unsigned count = roa_graph_node_count(graph_ctx);
    REQUIRE(count == 1);
  }

  SECTION("Remove nodes")
  {
    uint32_t new_node = roa_graph_node_create(graph_ctx);
    
    ROA_BOOL removed = roa_graph_node_remove(graph_ctx, new_node);

    unsigned count = roa_graph_node_count(graph_ctx);
    REQUIRE(count == 0);
    REQUIRE(removed == ROA_TRUE);
  }

  SECTION("Remove Invalid nodes")
  {
    uint32_t node_should_not_exist = 123;
    ROA_BOOL removed = roa_graph_node_remove(graph_ctx, node_should_not_exist);

    REQUIRE(removed == ROA_FALSE);
  }

  SECTION("Add child nodes")
  {
    uint32_t parent_node = roa_graph_node_create(graph_ctx);
    uint32_t child_node = roa_graph_node_create(graph_ctx);

    ROA_BOOL added_child = roa_graph_node_set_parent(graph_ctx, child_node, parent_node);
    REQUIRE(added_child == ROA_TRUE);

    unsigned count = roa_graph_node_count(graph_ctx);
    REQUIRE(count == 2);
  }
  
  SECTION("Get parent")
  {
    uint32_t parent_node = roa_graph_node_create(graph_ctx);
    uint32_t child_node = roa_graph_node_create(graph_ctx);

    roa_graph_node_set_parent(graph_ctx, child_node, parent_node);
    
    uint32_t parent = roa_graph_node_get_parent(graph_ctx, child_node);

    REQUIRE(parent == parent_node);
  }

  SECTION("Merge two trees")
  {
    REQUIRE(false);
  }

  roa_graph_ctx_destroy(&graph_ctx);
}