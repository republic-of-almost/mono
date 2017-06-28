#include <nil/data/developer.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Developer>&
get_developer_data()
{
  static Nil::Data::Generic_data<Nil::Data::Developer> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Developer **data)
{
  *count = get_developer_data().data.size();
  *data = get_developer_data().data.data();
}


void
get(const Node &node, Developer &out)
{
  get_developer_data().get_data(node, out);
}


void
set(Node &node, const Developer &in)
{
  get_developer_data().set_data(node, in);
}


void
remove_developer(Node &node)
{
  get_developer_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_developer(const Node &node)
{
  return get_developer_data().find(node);
}


bool
has(const Node &node, const Developer &)
{
  return has_developer(node);
}


uint64_t
get_type_id(const Developer &)
{
  return get_developer_data().type_id;
}


const char*
get_type_name(const Developer &in)
{
  return "Developer";
}


size_t
developer_count()
{
  return get_developer_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Developer **out_data, Node **out_node)
{
  return get_developer_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
