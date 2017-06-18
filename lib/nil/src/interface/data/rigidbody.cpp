#include <nil/data/rigidbody.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Rigidbody>&
get_rb_data()
{
  static Nil::Data::Generic_data<Nil::Data::Rigidbody> data;
  return data;
}


} // ns

namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Rigidbody &out)
{
  get_rb_data().get_data(node, out);
}


void
set(Node &node, const Rigidbody &in)
{
  get_rb_data().set_data(node, in);
}


void
remove_rigidbody(Node &node)
{
  get_rb_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --



bool
has_rigidbody(const Node &node)
{
  return get_rb_data().find(node);
}


uint64_t
get_type_id(const Rigidbody &)
{
  NIL_DATA_TYPE_ID_REG
}


size_t
rigidbody_count()
{
  return get_rb_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Rigidbody **out_data, Node **out_node)
{
  return get_rb_data().events(event, count, out_data, out_node);
}



} // ns
} // ns
