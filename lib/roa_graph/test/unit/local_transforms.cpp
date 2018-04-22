#include <roa_graph/roa_graph.h>
#include <catch/catch.hpp>
#include <roa_lib/log.h>
#include <roa_math/math.h>


TEST_CASE("Local transforms")
{
	roa_logging_set_output(0);

  roa_graph_ctx_t graph_ctx = ROA_NULL;
  roa_graph_ctx_create(&graph_ctx);

  SECTION("Inital Local Transform")
  {
    uint32_t node = roa_graph_node_create(graph_ctx);

    roa_transform transform;
    roa_graph_node_get_transform(graph_ctx, node, &transform, ROA_FALSE);

    roa_float3 expected_scale = roa_float3_one();
    REQUIRE(roa_float3_is_near(expected_scale, transform.scale, ROA_EPSILON) == ROA_TRUE);

    roa_float3 expected_position = roa_float3_zero();
    REQUIRE(roa_float3_is_near(expected_position, transform.position, ROA_EPSILON) == ROA_TRUE);

    roa_quaternion expected_rotation = roa_quaternion_default();
    REQUIRE(roa_quaternion_is_near(expected_rotation, transform.rotation, ROA_EPSILON) == ROA_TRUE);
  }

  SECTION("Inital World Transform")
  {
    uint32_t node = roa_graph_node_create(graph_ctx);

    roa_transform transform;
    roa_graph_node_get_transform(graph_ctx, node, &transform, ROA_TRUE);

    roa_float3 expected_scale = roa_float3_one();
    REQUIRE(roa_float3_is_near(expected_scale, transform.scale, ROA_EPSILON) == ROA_TRUE);

    roa_float3 expected_position = roa_float3_zero();
    REQUIRE(roa_float3_is_near(expected_position, transform.position, ROA_EPSILON) == ROA_TRUE);

    roa_quaternion expected_rotation = roa_quaternion_default();
    REQUIRE(roa_quaternion_is_near(expected_rotation, transform.rotation, ROA_EPSILON) == ROA_TRUE);
  }

  SECTION("Inherited World Transform")
  {
    /* setup parent */
    uint32_t parent = roa_graph_node_create(graph_ctx);
    {
      roa_transform parent_transform;
      parent_transform.position = roa_float3_fill_with_value(1.f);
      parent_transform.scale = roa_float3_fill_with_value(0.5f);

      roa_graph_node_set_transform(graph_ctx, parent, &parent_transform);
    }

    /* setup child */
    uint32_t child = roa_graph_node_create(graph_ctx);
    {
      roa_transform child_transform;
      child_transform.position = roa_float3_fill_with_value(1.f);
      child_transform.scale = roa_float3_fill_with_value(0.5f);

      roa_graph_node_set_transform(graph_ctx, child, &child_transform);

      roa_graph_node_set_parent(graph_ctx, child, parent);
    }

    /* get inherited parent transform */
    {
      //roa_transform transform;
      //roa_graph_node_get_transform(graph_ctx, parent, &transform, ROA_TRUE);

      //roa_float3 expected_scale = roa_float3_one();
      //REQUIRE(roa_float3_is_near(expected_scale, transform.scale, ROA_EPSILON) == ROA_TRUE);

      //roa_float3 expected_position = roa_float3_zero();
      //REQUIRE(roa_float3_is_near(expected_position, transform.position, ROA_EPSILON) == ROA_TRUE);

      //roa_quaternion expected_rotation = roa_quaternion_default();
      //REQUIRE(roa_quaternion_is_near(expected_rotation, transform.rotation, ROA_EPSILON) == ROA_TRUE);
    }

    /* get inherited child transform */
    {
      roa_transform child_transform;
      roa_graph_node_get_transform(graph_ctx, child, &child_transform, ROA_TRUE);
    }
  }

  roa_graph_ctx_destroy(&graph_ctx);
}
