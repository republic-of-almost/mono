#include <roa_graph/roa_graph.h>
#include <catch/catch.hpp>


TEST_CASE("Nodes")
{
  roa_graph_ctx_t graph_ctx = ROA_NULL;
  roa_graph_ctx_create(&graph_ctx);
  
  SECTION("inital state")
  {
    unsigned count = roa_graph_node_child_count(graph_ctx, 0);
    REQUIRE(count == 0);
  }

  SECTION("create node")
  {
    uint32_t id = roa_graph_node_create(graph_ctx);
    REQUIRE(id >= 0);

    ROA_BOOL valid = roa_graph_node_exists(graph_ctx, id, ROA_NULL);
    REQUIRE(valid == ROA_TRUE);

    uint32_t count = roa_graph_node_child_count(graph_ctx, 0);
    REQUIRE(count >= 0);
    
    uint32_t id_child_count = roa_graph_node_child_count(graph_ctx, id);
    REQUIRE(id_child_count == 0);
  }

  SECTION("delete node")
  {
    uint32_t id = roa_graph_node_create(graph_ctx);

    roa_graph_node_remove(graph_ctx, id);

    ROA_BOOL valid = roa_graph_node_exists(graph_ctx, id, ROA_NULL);
    REQUIRE(valid == ROA_FALSE);
  }

  SECTION("create child node")
  {
    uint32_t parent = roa_graph_node_create(graph_ctx);
    uint32_t child = roa_graph_node_create(graph_ctx);
  
    ROA_BOOL parented = roa_graph_node_set_parent(graph_ctx, child, parent);
    REQUIRE(parented == ROA_TRUE);

    unsigned child_count = roa_graph_node_child_count(graph_ctx, parent);
    REQUIRE(child_count == 1);

    uint32_t get_parent = roa_graph_node_get_parent(graph_ctx, child);
    REQUIRE(parent == get_parent);
  }

  SECTION("remove child node")
  {
    uint32_t parent = roa_graph_node_create(graph_ctx);
    uint32_t child = roa_graph_node_create(graph_ctx);

    roa_graph_node_set_parent(graph_ctx, child, parent);

    uint32_t get_parent = roa_graph_node_get_parent(graph_ctx, child);
    REQUIRE(parent == get_parent);

    roa_graph_node_set_parent(graph_ctx, child, 0);
    uint32_t get_parent_again = roa_graph_node_get_parent(graph_ctx, child);
    REQUIRE(get_parent_again == 0);
  }

  SECTION("delete parent with child")
  {
    uint32_t parent = roa_graph_node_create(graph_ctx);
    uint32_t child = roa_graph_node_create(graph_ctx);

    roa_graph_node_set_parent(graph_ctx, child, parent);

    roa_graph_node_remove(graph_ctx, parent);

    ROA_BOOL parent_valid = roa_graph_node_exists(graph_ctx, parent, ROA_NULL);
    REQUIRE(parent_valid == ROA_FALSE);

    ROA_BOOL child_valid = roa_graph_node_exists(graph_ctx, child, ROA_NULL);
    REQUIRE(child_valid == ROA_FALSE);
  }

  SECTION("workflow: parent - child - gran-child - remove middle child")
  {
    /* setup */

    uint32_t parent = roa_graph_node_create(graph_ctx);
    uint32_t child = roa_graph_node_create(graph_ctx);
    uint32_t gran_child = roa_graph_node_create(graph_ctx);

    roa_graph_node_set_parent(graph_ctx, child, parent);
    roa_graph_node_set_parent(graph_ctx, gran_child, child);

    /* check relationships */
    {
      uint32_t parent_children = roa_graph_node_child_count(graph_ctx, parent);
      REQUIRE(parent_children == 1);

      uint32_t parent_decendents = roa_graph_node_descendants_count(graph_ctx, parent);
      REQUIRE(parent_decendents == 2);

      uint32_t child_children = roa_graph_node_child_count(graph_ctx, child);
      REQUIRE(child_children == 1);

      uint32_t child_decendents = roa_graph_node_descendants_count(graph_ctx, child);
      REQUIRE(child_decendents == 1);

      uint32_t gran_childs_children = roa_graph_node_child_count(graph_ctx, gran_child);
      REQUIRE(gran_childs_children == 0);

      uint32_t gran_child_decendents = roa_graph_node_descendants_count(graph_ctx, gran_child);
      REQUIRE(gran_child_decendents == 0);
    }

    /* remove middle child and recheck relation ships */
    {
      roa_graph_node_set_parent(graph_ctx, child, 0);

      uint32_t parent_children = roa_graph_node_child_count(graph_ctx, parent);
      REQUIRE(parent_children == 0);

      uint32_t parent_decendents = roa_graph_node_descendants_count(graph_ctx, parent);
      REQUIRE(parent_decendents == 0);

      uint32_t child_children = roa_graph_node_child_count(graph_ctx, child);
      REQUIRE(child_children == 1);

      uint32_t child_decendents = roa_graph_node_descendants_count(graph_ctx, child);
      REQUIRE(child_decendents == 1);

      uint32_t gran_childs_children = roa_graph_node_child_count(graph_ctx, gran_child);
      REQUIRE(gran_childs_children == 0);

      uint32_t gran_child_decendents = roa_graph_node_descendants_count(graph_ctx, gran_child);
      REQUIRE(gran_child_decendents == 0);
    }
  }

  roa_graph_ctx_destroy(&graph_ctx);
}
