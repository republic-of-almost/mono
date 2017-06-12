#include <nil/data/window.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


namespace Nil {
namespace Data {


void
get(const Node &node, Window &out)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP

  if(!getter_helper(
        node.get_id(),
        graph->component_data.window_node_id,
        graph->component_data.window_data,
        out))
  {
    NIL_DATA_GETTER_ERROR(Window)
  }
}


void
set(Node &node, const Window &in)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP

  if(!setter_helper(
    node,
    graph->component_data.window_node_id,
    graph->component_data.window_data,
    in,
    get_type_id(in)))
  {
    NIL_DATA_SETTER_ERROR(Window)
  }
}


bool
has_window(const Node &node)
{
  NIL_DATA_GETTER_SETTER_HAS_SETUP
  
  return has(node.get_id(), graph->component_data.window_node_id);
}


uint64_t
get_type_id(const Window &)
{
  NIL_DATA_TYPE_ID_REG
}


} // ns
} // ns
