#include <nil/data/logic.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Logic>&
get_logic_data()
{
  static Nil::Data::Generic_data<Nil::Data::Logic> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Logic **data)
{
  *count = get_logic_data().data.size();
  *data = get_logic_data().data.data();
}


void
get(const Node &node, Logic &out)
{
  get_logic_data().get_data(node, out);
}


void
set(Node &node, const Logic &in)
{
  get_logic_data().set_data(node, in);
}


void
remove_logic(Node &node)
{
  get_logic_data().remove_data(node);
}



// ----------------------------------------------------------------- [ Info ] --


bool
has_logic(const Node &node)
{
  return get_logic_data().find(node);
}


bool
has(const Node &node, const Logic &)
{
  return has_logic(node);
}


uint64_t
get_type_id(const Logic &)
{
  return get_logic_data().type_id;
}


const char*
get_type_name(const Logic &in)
{
  return "Logic";
}


size_t
logic_count()
{
  return get_logic_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Logic **out_data, Node **out_node)
{
  return get_logic_data().events(event, count, out_data, out_node);
}




} // ns
} // ns
