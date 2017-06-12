#include <nil/data/bounding_box.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph.hpp>
#include <string.h>
#include "common.hpp"


namespace Nil {
namespace Data {


void
get(const Node &node, Bounding_box &out)
{
  if(node.is_valid())
  {
    math::aabb internal;
    if(Graph::node_get_bounding_box(Data::get_graph_data(), node.get_id(), &internal))
    {
      memcpy(out.min, &internal.min, sizeof(float) * 3);
      memcpy(out.max, &internal.max, sizeof(float) * 3);
    }
    else
    {
      LIB_ASSERT(false);
      LOG_ERROR("Something went wrong getting bounding box");
    }
  }
  else
  {
    LIB_ASSERT(false);
    LOG_ERROR("Invalid Node");
  }
}


void
set(Node &node, const Bounding_box &in)
{
  if(node.is_valid())
  {
    math::aabb internal;
    memcpy(&internal.min, in.min, sizeof(float) * 3);
    memcpy(&internal.max, in.max, sizeof(float) * 3);
    
    if(!Graph::node_set_bounding_box(Data::get_graph_data(), node.get_id(), &internal))
    {
      LIB_ASSERT(false);
      LOG_ERROR("Something went wrong setting bounding box");
    }
  }
  else
  {
    LIB_ASSERT(false);
    LOG_ERROR("Invalid Node");
  }
}


bool
has_bounding_box(const Node &)
{
  return true;
}


uint64_t
get_type_id(const Bounding_box &)
{
  NIL_DATA_TYPE_ID_REG
}


} // ns
} // ns
