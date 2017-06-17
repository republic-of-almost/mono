#include <nil/data/window.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include <lib/optimizations.hpp>
#include <stdint.h>
#include "common.hpp"


namespace {


// -- Lazy Getter -- //


Nil::Data::Generic_data<Nil::Data::Window>&
get_win_data()
{
  static Nil::Data::Generic_data<Nil::Data::Window> win;
  return win;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


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
  return get_win_data().find(node);
}


uint64_t
get_type_id(const Window &)
{
  NIL_DATA_TYPE_ID_REG
}


size_t
window_count()
{
  return get_win_data().keys.size();
}


// ------------------------------------------------------- [ Updated Events ] --


void
events(const uint32_t event, size_t *count, Window **out_win, Node **out_node)
{
  return get_win_data().events(event, count, out_win, out_node);
}


} // ns
} // ns
