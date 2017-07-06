#include <nil/data/bounding_box.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph.hpp>
#include <string.h>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


struct Bounding_box_data
{
  explicit Bounding_box_data()
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
Bounding_box_data
get_bbox_data()
{
  static Bounding_box_data data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


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
  
  Graph::data_updated(Data::get_graph_data(), node.get_id(), get_bbox_data().type_id, true);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_bounding_box(const Node &)
{
  return true;
}


bool
has(const Node &node, const Bounding_box &)
{
  return has_bounding_box(node);
}


uint64_t
get_type_id(const Bounding_box &)
{
  return get_bbox_data().type_id;
}


const char*
get_type_name(const Bounding_box &in)
{
  return "Bounding_box";
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Bounding_box_data **out_data, Node **out_node)
{
  LIB_ASSERT(false); // Not impl'd unsure if needed right now!
}


} // ns
} // ns
