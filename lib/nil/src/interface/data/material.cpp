#include <nil/data/material.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Material>&
get_mat_data()
{
  static Nil::Data::Generic_data<Nil::Data::Material> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Material &out)
{
  get_mat_data().get_data(node, out);
}


void
set(Node &node, const Material &in)
{
  get_mat_data().set_data(node, in);
}


void
remove_material(Node &node)
{
  get_mat_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_material(const Node &node)
{
  return get_mat_data().find(node);
}


uint64_t
get_type_id(const Material &)
{
  NIL_DATA_TYPE_ID_REG
}


size_t
material_count()
{
  return get_mat_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Material **out_data, Node **out_node)
{
  return get_mat_data().events(event, count, out_data, out_node);
}



} // ns
} // ns
