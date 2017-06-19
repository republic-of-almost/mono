#include <nil/data/light.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Light>&
get_light_data()
{
  static Nil::Data::Generic_data<Nil::Data::Light> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Light &out)
{
  get_light_data().get_data(node, out);
}


void
set(Node &node, const Light &in)
{
  get_light_data().set_data(node, in);
}


void
remove_light(Node &node)
{
  get_light_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_light(const Node &node)
{
  return get_light_data().find(node);
}


uint64_t
get_type_id(const Light &)
{
  return get_light_data().type_id;
}


size_t
light_count()
{
  return get_light_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Light **out_data, Node **out_node)
{
  return get_light_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
