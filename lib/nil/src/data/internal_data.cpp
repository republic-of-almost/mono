#include "internal_data.hpp"
#include <graph/graph_data.hpp>
#include <nil/node.hpp>
#include <lib/bits.hpp>


namespace Nil {
namespace Data {


Graph::Data*
get_graph_data()
{
  static Graph::Data graph;
  return &graph;
}


Node&
get_root_node()
{
  static Node node(uint32_t{0});
  return node;
}


} // ns
} // ns
