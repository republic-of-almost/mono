#include <nil/data/light.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


namespace Nil {
namespace Data {


void
get(const Node &node, Light &out)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP

  if(!getter_helper(
        node.get_id(),
        graph->component_data.light_node_id,
        graph->component_data.light_data,
        out))
  {
    NIL_DATA_GETTER_ERROR(Light)
  }
}


void
set(Node &node, const Light &in)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP

  if(!setter_helper(
    node,
    graph->component_data.light_node_id,
    graph->component_data.light_data,
    in,
    get_type_id(in)))
  {
    NIL_DATA_SETTER_ERROR(Light)
  }
}


bool
has_light(const Node &node)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP
  
  return has(node.get_id(), graph->component_data.light_node_id);
}


uint64_t
get_type_id(const Light &)
{
  NIL_DATA_TYPE_ID_REG
}


} // ns
} // ns
