#include <nil/data/resource.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Resource>&
get_rsrc_data()
{
  static Nil::Data::Generic_data<Nil::Data::Resource> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Resource **data)
{
  *count = get_rsrc_data().data.size();
  *data = get_rsrc_data().data.data();
}


void
get(const Node &node, Resource &out)
{
  get_rsrc_data().get_data(node, out);
}


void
set(Node &node, const Resource &in)
{
  get_rsrc_data().set_data(node, in);
}


void
remove_resource(Node &node)
{
  get_rsrc_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_resource(const Node &node)
{
  return get_rsrc_data().find(node);
}


bool
has(const Node &node, const Resource &)
{
  return has_resource(node);
}


uint64_t
get_type_id(const Resource &)
{
  return get_rsrc_data().type_id;
}


const char*
get_type_name(const Resource &in)
{
  return "Resource";
}


size_t
resource_count()
{
  return get_rsrc_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Resource **out_data, Node **out_node)
{
  return get_rsrc_data().events(event, count, out_data, out_node);
}



} // ns
} // ns
