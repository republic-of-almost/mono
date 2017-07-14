#include <nil/data/gamepad.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
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
get(size_t *count, Gamepad **data)
{
  *count = get_gamepad_data().data.size();
  *data = get_gamepad_data().data.data();
}


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
  return find_node(
    node,
    get_gamepad_data().keys.data(),
    get_gamepad_data().keys.size()
  );

}


bool
has(const Node &node, const Gamepad &)
{
  return has_gamepad(node);
}


uint64_t
get_type_id(const Gamepad &)
{
  return get_gamepad_data().type_id;
}


const char*
get_type_name(const Gamepad &in)
{
  return "Gamepad";
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
