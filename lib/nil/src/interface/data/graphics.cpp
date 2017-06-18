#include <nil/data/graphics.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Graphics>&
get_graphics_data()
{
  static Nil::Data::Generic_data<Nil::Data::Graphics> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Graphics &out)
{
  get_graphics_data().get_data(node, out);
}


void
set(Node &node, const Graphics &in)
{
  get_graphics_data().set_data(node, in);
}


void
remove_graphics(Node &node)
{
  get_graphics_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_graphics(const Node &node)
{
  return get_graphics_data().find(node);
}


uint64_t
get_type_id(const Graphics &)
{
  NIL_DATA_TYPE_ID_REG
}


size_t
graphics_count()
{
  return get_graphics_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Graphics **out_data, Node **out_node)
{
  return get_graphics_data().events(event, count, out_data, out_node);
}





} // ns
} // ns
