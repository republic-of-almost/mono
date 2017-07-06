#include <nil/data/transform.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph.hpp>
#include <string.h>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


struct Transform_data
{
  explicit Transform_data()
  {
    type_id = Nil::Graph::data_register_type(
      Nil::Data::get_graph_data(),
      nullptr,
      nullptr,
      nullptr,
      (uintptr_t)this,
      0
    );
  }
  
  uint64_t type_id;
};


// -- Lazy Getter -- //
Transform_data
get_trans_data()
{
  static Transform_data data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


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
  
  Graph::data_updated(Data::get_graph_data(), node.get_id(), get_trans_data().type_id, true);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_transform(const Node &)
{
  return true;
}


bool
has(const Node &node, const Transform &)
{
  return has_transform(node);
}


uint64_t
get_type_id(const Transform &)
{
  return get_trans_data().type_id;
}


const char*
get_type_name(const Transform &in)
{
  return "Transform";
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Transform **out_data, Node **out_node)
{
  LIB_ASSERT(false); // Not impl'd unsure if needed right now!
}


} // ns
} // ns
