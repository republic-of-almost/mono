#include <nil/data/mouse.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
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
get(size_t *count, Mouse **data)
{
  *count = get_ms_data().data.size();
  *data = get_ms_data().data.data();
}


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
  return find_node(
    node,
    get_ms_data().keys.data(),
    get_ms_data().keys.size()
  );

}


bool
has(const Node &node, const Mouse &)
{
  return has_mouse(node);
}


uint64_t
get_type_id(const Mouse &)
{
  return get_ms_data().type_id;
}


const char *
get_type_name(const Mouse &in)
{
  return "Mouse";
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
