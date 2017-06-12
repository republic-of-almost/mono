#include <nil/data/logic.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


namespace Nil {
namespace Data {


void
get(const Node &node, Logic &out)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP

  if(!getter_helper(
        node.get_id(),
        graph->component_data.logic_node_id,
        graph->component_data.logic_data,
        out))
  {
    NIL_DATA_GETTER_ERROR(Logic)
  }
}


void
set(Node &node, const Logic &in)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP

  if(!setter_helper(
    node,
    graph->component_data.logic_node_id,
    graph->component_data.logic_data,
    in,
    get_type_id(in)))
  {
    NIL_DATA_SETTER_ERROR(Logic)
  }
}


bool
has_logic(const Node &node)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP
  
  return has(node.get_id(), graph->component_data.logic_node_id);
}


uint64_t
get_type_id(const Logic &)
{
  NIL_DATA_TYPE_ID_REG
}


} // ns
} // ns
