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
Nil::Data::Generic_data<Nil::Data::Bounding_box>&
get_bb_data()
{
  static Nil::Data::Generic_data<Nil::Data::Bounding_box> data(
    Nil::Data::get_type_id(Nil::Data::Transform{}),
    [](uint32_t id, uintptr_t user_data, size_t index)
    {
      Nil::Data::Generic_data<Nil::Data::Camera> *data = reinterpret_cast<Nil::Data::Generic_data<Nil::Data::Camera>*>(user_data);
      LIB_ASSERT(data);
      
      Nil::Node node(id);
      
      Nil::Data::Transform trans;
      Nil::Data::get(node, trans, true);
      
      Nil::Data::Bounding_box bb;
      Nil::Data::get(node, bb);

      math::aabb aabb = math::aabb_init(
        math::vec3_init(bb.min),
        math::vec3_init(bb.max)
      );
    }
  );
  
  return data;
}


} // anon ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Bounding_box &out, const bool inherited)
{

}


void
set(Node &node, const Bounding_box &in)
{

}


// ----------------------------------------------------------------- [ Info ] --


bool
has_bounding_box(const Node &)
{
  
}


bool
has(const Node &node, const Bounding_box &)
{
  
}


uint64_t
get_type_id(const Bounding_box &)
{
  
}


const char*
get_type_name(const Bounding_box &in)
{
  
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Bounding_box_data **out_data, Node **out_node)
{
  
}


} // ns
} // ns
