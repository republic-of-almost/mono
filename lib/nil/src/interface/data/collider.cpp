#include <nil/data/collider.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Collider>&
get_collider_data()
{
  static Nil::Data::Generic_data<Nil::Data::Collider> data;
  return data;
}


} // ns



namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Collider &out)
{
  get_collider_data().get_data(node, out);
}


void
set(Node &node, const Collider &in)
{
  get_collider_data().set_data(node, in);
}


void
remove_collider(Node &node)
{
  get_collider_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_collider(const Node &node)
{
  return get_collider_data().find(node);
}


uint64_t
get_type_id(const Collider &)
{
  NIL_DATA_TYPE_ID_REG
}


size_t
collider_count()
{
  return get_collider_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Collider **out_data, Node **out_node)
{
  return get_collider_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
