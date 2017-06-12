#include "data.hpp"
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


uint64_t
register_type_id()
{
  static int i = 0;
  
  if(i >= 64)
  {
    LIB_ASSERT(false);
    LOG_FATAL("Run out of data type identifiers, the max is 64");
    return 0;
  }
  
  uint64_t new_id = BIT(i);
  
  ++i;
  
  return new_id;
}


} // ns
} // ns
