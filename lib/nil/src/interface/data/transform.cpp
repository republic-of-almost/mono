#include <nil/data/transform.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph.hpp>
#include <string.h>
#include "common.hpp"


namespace Nil {
namespace Data {


void
get(const Node &node, Transform &out, const bool world)
{
  if(node.is_valid())
  {
    math::transform internal;
    if(Graph::node_get_transform(Data::get_graph_data(), node.get_id(), &internal, world))
    {
      memcpy(out.position, &internal.position, sizeof(float) * 3);
      memcpy(out.scale,    &internal.scale,    sizeof(float) * 3);
      memcpy(out.rotation, &internal.rotation, sizeof(float) * 4);
    }
    else
    {
      LIB_ASSERT(false);
      LOG_ERROR("Something went wrong getting transform");
    }
  }
  else
  {
    LIB_ASSERT(false);
    LOG_ERROR("Invalid Node");
  }
}


void
set(Node &node, const Transform &in)
{
  if(node.is_valid())
  {
    math::transform internal;
    memcpy(&internal.position, in.position, sizeof(float) * 3);
    memcpy(&internal.scale,    in.scale,    sizeof(float) * 3);
    memcpy(&internal.rotation, in.rotation, sizeof(float) * 4);
    
    if(!Graph::node_set_transform(Data::get_graph_data(), node.get_id(), &internal))
    {
      LIB_ASSERT(false);
      LOG_ERROR("Something went wrong setting transform");
    }
  }
  else
  {
    LIB_ASSERT(false);
    LOG_ERROR("Invalid Node");
  }
}


bool
has_transform(const Node &)
{
  return true;
}


uint64_t
get_type_id(const Transform &)
{
  NIL_DATA_TYPE_ID_REG
}


} // ns
} // ns
