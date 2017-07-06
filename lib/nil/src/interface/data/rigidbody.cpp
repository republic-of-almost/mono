#include <nil/data/rigidbody.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
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
get(size_t *count, Rigidbody **data)
{
  *count = get_rb_data().data.size();
  *data = get_rb_data().data.data();
}


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


bool
has(const Node &node, const Rigidbody &)
{
  return has_rigidbody(node);
}


uint64_t
get_type_id(const Rigidbody &)
{
  return get_rb_data().type_id;
}


const char *
get_type_name(const Rigidbody &in)
{
  return "Rigidbody";
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
