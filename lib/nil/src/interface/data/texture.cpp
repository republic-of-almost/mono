#include <nil/data/texture.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Texture>&
get_texture_data()
{
  static Nil::Data::Generic_data<Nil::Data::Texture> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Texture &out)
{
  get_texture_data().get_data(node, out);
}


void
set(Node &node, const Texture &in)
{
  get_texture_data().set_data(node, in);
}


void
remove_texture(Node &node)
{
  get_texture_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_texture(const Node &node)
{
  return get_texture_data().find(node);
}


uint64_t
get_type_id(const Texture &)
{
  return get_texture_data().type_id;
}


size_t
texture_count()
{
  return get_texture_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Texture **out_data, Node **out_node)
{
  return get_texture_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
