#include <nil/data/window.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include <lib/optimizations.hpp>
#include <stdint.h>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Window>&
get_win_data()
{
  static Nil::Data::Generic_data<Nil::Data::Window> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *out_count, Window **out_resource)
{
  *out_count    = get_win_data().data.size();
  *out_resource = get_win_data().data.data();
}


void
get(const Node &node, Window &out)
{
  get_win_data().get_data(node, out);
}


void
set(Node &node, const Window &in)
{
  get_win_data().set_data(node, in);
}


void
remove_window(Node &node)
{
  get_win_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_window(const Node &node)
{
  return find_node(
    node,
    get_win_data().keys.data(),
    get_win_data().keys.size()
  );

}


bool
has(const Node &node, const Window &)
{
  return has_window(node);
}


uint64_t
get_type_id(const Window &)
{
  return get_win_data().type_id;
}


const char*
get_type_name(const Window &in)
{
  return "Window";
}


size_t
window_count()
{
  return get_win_data().keys.size();
}


// ------------------------------------------------------- [ Updated Events ] --


void
events(const uint32_t event, size_t *count, Window **out_data, Node **out_node)
{
  return get_win_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
