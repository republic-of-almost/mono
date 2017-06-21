#include <nil/data/keyboard.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Keyboard>&
get_kb_data()
{
  static Nil::Data::Generic_data<Nil::Data::Keyboard> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Keyboard **data)
{
  *count = get_kb_data().data.size();
  *data = get_kb_data().data.data();
}


void
get(const Node &node, Keyboard &out)
{
  get_kb_data().get_data(node, out);
}


void
set(Node &node, const Keyboard &in)
{
  get_kb_data().set_data(node, in);
}


void
remove_keyboard(Node &node)
{
  get_kb_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_keyboard(const Node &node)
{
  return get_kb_data().find(node);
}


uint64_t
get_type_id(const Keyboard &)
{
  return get_kb_data().type_id;
}


size_t
keyboard_count()
{
  return get_kb_data().keys.size();
}


// ------------------------------------------------------- [ Updated Events ] --


void
events(const uint32_t event, size_t *count, Keyboard **out_data, Node **out_node)
{
  return get_kb_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
