#include <nil/data/texture.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
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
get(size_t *count, Texture **data)
{
  *count = get_texture_data().data.size();
  *data = get_texture_data().data.data();
}


void
get(const Node &node, Texture &out)
{
  get_texture_data().get_data(node, out);
}


void
set(Node &node, const Texture &in)
{
  Texture cpy_in = in;
  
  if(in.id == 0)
  {
    const_cast<Texture&>(in).id = get_texture_data().data.size();
   
    void *owned_mem = malloc(in.data_size);
    memcpy(owned_mem, (void*)in.data, in.data_size);
    
    cpy_in.data = (uintptr_t)owned_mem;
  }
  get_texture_data().set_data(node, cpy_in);
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


bool
has(const Node &node, const Texture &)
{
  return has_texture(node);
}


uint64_t
get_type_id(const Texture &)
{
  return get_texture_data().type_id;
}


const char*
get_type_name(const Texture &in)
{
  return "Texture";
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
