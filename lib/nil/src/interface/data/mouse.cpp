#include <nil/data/mouse.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Mouse>&
get_ms_data()
{
  static Nil::Data::Generic_data<Nil::Data::Mouse> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Mouse &out)
{
  get_ms_data().get_data(node, out);
}


void
set(Node &node, const Mouse &in)
{
  get_ms_data().set_data(node, in);
}


void
remove_mouse(Node &node)
{
  get_ms_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_mouse(const Node &node)
{
  return get_ms_data().find(node);
}


uint64_t
get_type_id(const Mouse &)
{
  NIL_DATA_TYPE_ID_REG
}


size_t
mouse_count()
{
  return get_ms_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Mouse **out_data, Node **out_node)
{
  return get_ms_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
