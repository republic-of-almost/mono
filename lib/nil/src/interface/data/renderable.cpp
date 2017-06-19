#include <nil/data/renderable.hpp>
#include <nil/data/transform.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Renderable>&
get_mat_data()
{
  static Nil::Data::Generic_data<Nil::Data::Renderable> data(
    Nil::Data::get_type_id(Nil::Data::Transform{})
  );
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Renderable &out)
{
  get_mat_data().get_data(node, out);
}


void
set(Node &node, const Renderable &in)
{
  get_mat_data().set_data(node, in);
}


void
remove_renderable(Node &node)
{
  get_mat_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_renderable(const Node &node)
{
  return get_mat_data().find(node);
}


uint64_t
get_type_id(const Renderable &)
{
  return get_mat_data().type_id;
}


size_t
renderable_count()
{
  return get_mat_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Renderable **out_data, Node **out_node)
{
  return get_mat_data().events(event, count, out_data, out_node);
}



} // ns
} // ns
