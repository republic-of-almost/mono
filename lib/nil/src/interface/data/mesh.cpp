#include <nil/data/mesh.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Mesh>&
get_mesh_data()
{
  static Nil::Data::Generic_data<Nil::Data::Mesh> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Mesh **data)
{
  *count = get_mesh_data().data.size();
  *data = get_mesh_data().data.data();
}


void
get(const Node &node, Mesh &out)
{
  get_mesh_data().get_data(node, out);
}


void
set(Node &node, const Mesh &in)
{
  get_mesh_data().set_data(node, in);
}


void
remove_mesh(Node &node)
{
  get_mesh_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_mesh(const Node &node)
{
  return get_mesh_data().find(node);
}


bool
has(const Node &node, const Mesh &)
{
  return has_mesh(node);
}


uint64_t
get_type_id(const Mesh &)
{
  return get_mesh_data().type_id;
}


const char*
get_type_name(const Mesh &in)
{
  return "Mesh";
}


size_t
mesh_count()
{
  return get_mesh_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Mesh **out_data, Node **out_node)
{
  return get_mesh_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
