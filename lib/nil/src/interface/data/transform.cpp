#include <nil/data/transform.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph.hpp>
#include <math/transform/transform.hpp>
#include <lib/assert.hpp>
#include <lib/logging.hpp>
#include <string.h>


namespace {


// ----------------------------------------------------------------- [ Data ] --


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


// ---------------------------------------------------------- [ Identifiers ] --


constexpr char transform_name[] = "Transform";


// ------------------------------------------------------------- [ Messages ] --


constexpr char msg_transform_failed_to_find[] = "Failed to located transform";
constexpr char msg_transform_no_node[]        = "Transform with ID: %d is invalid";


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Transform &out, const bool inherited)
{
  if(node.is_valid())
  {
    math::transform internal;
    if(Graph::node_get_transform(Data::get_graph_data(), node.get_id(), &internal, inherited))
    {
      memcpy(out.position, &internal.position, sizeof(float) * 3);
      memcpy(out.scale,    &internal.scale,    sizeof(float) * 3);
      memcpy(out.rotation, &internal.rotation, sizeof(float) * 4);
    }
    else
    {
      LIB_ASSERT(false);
      LOG_ERROR(msg_transform_failed_to_find);
    }
  }
  else
  {
    LIB_ASSERT(false);
    LOG_ERROR(msg_transform_no_node, node.get_id());
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
      LOG_ERROR(msg_transform_failed_to_find);
    }
  }
  else
  {
    LIB_ASSERT(false);
    LOG_ERROR(msg_transform_no_node, node.get_id());
  }
  
  Graph::data_updated(Data::get_graph_data(), node.get_id(), get_trans_data().type_id);
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
  return transform_name;
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Transform **out_data, Node **out_node)
{
  LIB_ASSERT(false); // Not impl'd unsure if needed right now!
}


} // ns
} // ns
