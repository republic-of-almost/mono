#include <nil/data/gamepad.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Gamepad>&
get_gamepad_data()
{
  static Nil::Data::Generic_data<Nil::Data::Gamepad> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Gamepad &out)
{
  get_gamepad_data().get_data(node, out);
}


void
set(Node &node, const Gamepad &in)
{
  get_gamepad_data().set_data(node, in);
}


void
remove_gamepad(Node &node)
{
  get_gamepad_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_gamepad(const Node &node)
{
  return get_gamepad_data().find(node);
}


uint64_t
get_type_id(const Gamepad &)
{
  NIL_DATA_TYPE_ID_REG
}


size_t
gamepad_count()
{
  return get_gamepad_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Gamepad **out_data, Node **out_node)
{
  return get_gamepad_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
