#include <nil/data/texture_resource.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Texture_resource>&
get_texture_rsrc_data()
{
  static Nil::Data::Generic_data<Nil::Data::Texture_resource> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *out_count, Texture_resource **out_resource)
{
  *out_count    = get_texture_rsrc_data().data.size();
  *out_resource = get_texture_rsrc_data().data.data();
}


void
get(const Node &node, Texture_resource &out)
{
  get_texture_rsrc_data().get_data(node, out);
}


void
set(Node &node, const Texture_resource &in)
{
  Texture_resource cpy_in = in;
  cpy_in.data = nullptr;
  
  if(cpy_in.status == 0)
  {
    uint8_t *cpy_data = (uint8_t*)malloc(in.sizeof_data);
    
    if(cpy_data)
    {
      memcpy(&cpy_data[0], in.data, in.sizeof_data);
      cpy_in.data = cpy_data;
    }
		  }

  get_texture_rsrc_data().set_data(node, cpy_in);
}


void
remove_texture_resource(Node &node)
{
  get_texture_rsrc_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_texture_resource(const Node &node)
{
  return get_texture_rsrc_data().find(node);
}


bool
has(const Node &node, const Texture_resource &)
{
  return has_texture_resource(node);
}


uint64_t
get_type_id(const Texture_resource &)
{
  return get_texture_rsrc_data().type_id;
}


const char*
get_type_name(const Texture_resource &in)
{
  return "Texture_resource";
}


size_t
texture_resource_count()
{
  return get_texture_rsrc_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Texture_resource **out_data, Node **out_node)
{
  return get_texture_rsrc_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
