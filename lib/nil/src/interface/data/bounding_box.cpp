#include <nil/data/bounding_box.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph.hpp>
#include <math/geometry/geometry.hpp>
#include <string.h>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


struct Bounding_box_data
{
  lib::array<uint32_t> keys;
  lib::array<uint32_t> actions;
  
  lib::array<Nil::Data::Bounding_box> local_bb;
  lib::array<Nil::Data::Bounding_box> world_bb;
  
  uint64_t type_id;
  
  // -- Cached Event Data -- //
  
  std::vector<Nil::Node>              added_bb_nodes;
  lib::array<Nil::Data::Bounding_box> added_bb_data;

  std::vector<Nil::Node>              updated_bb_nodes;
  lib::array<Nil::Data::Bounding_box> updated_bb_data;
  
  std::vector<Nil::Node>              removed_bb_nodes;
  lib::array<Nil::Data::Bounding_box> removed_bb_data;
  
  // --
  
  Nil::Data::setter_cb setter_fn = nullptr;
  
  explicit
  Bounding_box_data()
  {
  }
  
};


// -- Lazy Getter -- //
Bounding_box_data&
get_bb_data()
{
  static Bounding_box_data bb;
  return bb;
}


} // anon ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Bounding_box &out, const bool world)
{
  get_data_helper(
    node,
    out,
    get_bb_data().keys.data(),
    get_bb_data().keys.size(),
    (uintptr_t)&world,
    
    // Found
    [](const size_t index, Bounding_box &out, uintptr_t user_data)
    {
      const bool world = *(bool*)user_data;
      
      if(world)
      {
        out = get_bb_data().world_bb[index];
      }
      else
      {
        out = get_bb_data().local_bb[index];
      }
    },
    
    // Not Found
    [](const uint32_t node_id, Bounding_box &out, uintptr_t user_data)
    {
      LOG_ERROR("Failed to fnid data")
    }
  );
}


void
set(Node &node, const Bounding_box &in)
{
  set_data_helper(
    node,
    in,
    get_bb_data().keys.data(),
    get_bb_data().keys.size(),
    get_bb_data().type_id,
    (uintptr_t)&node,
    
    // Updated existing data
    [](const size_t index, const Bounding_box &in, uintptr_t user_data)
    {
      Node *node = reinterpret_cast<Node*>(user_data);
      
      Nil::Data::Transform trans;
      Nil::Data::get(*node, trans);
      
      const math::vec3 pos = math::vec3_init(trans.position);
      const math::vec3 min = math::vec3_add(math::vec3_init(in.min), pos);
      const math::vec3 max = math::vec3_add(math::vec3_init(in.max), pos);
      
      Nil::Data::Bounding_box world_in = in;
      memcpy(world_in.min, min.data, sizeof(world_in.min));
      memcpy(world_in.max, max.data, sizeof(world_in.max));
    
      get_bb_data().local_bb[index] = in;
      get_bb_data().world_bb[index] = world_in;
    },
    
    // Set new data
    [](const uint32_t node_id, const Bounding_box &in, uintptr_t user_data)
    {
      Node node(node_id);
      
      Nil::Data::Transform trans;
      Nil::Data::get(node, trans);
      
      const math::vec3 pos = math::vec3_init(trans.position);
      const math::vec3 min = math::vec3_add(math::vec3_init(in.min), pos);
      const math::vec3 max = math::vec3_add(math::vec3_init(in.max), pos);
      
      Nil::Data::Bounding_box world_in = in;
      memcpy(world_in.min, min.data, sizeof(world_in.min));
      memcpy(world_in.max, max.data, sizeof(world_in.max));
      
      get_bb_data().local_bb.emplace_back(in);
      get_bb_data().world_bb.emplace_back(world_in);
    }
  );
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_bounding_box(const Node &node)
{
  return find_node(node, get_bb_data().keys.data(), get_bb_data().keys.size());
}


bool
has(const Node &node, const Bounding_box &)
{
  return has_bounding_box(node);
}


uint64_t
get_type_id(const Bounding_box &)
{
  return get_bb_data().type_id;
}


const char*
get_type_name(const Bounding_box &in)
{
  return "BoundingBox";
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Bounding_box_data **out_data, Node **out_node)
{
  
}


} // ns
} // ns
