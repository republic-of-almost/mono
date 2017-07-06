#include <nil/data/audio_resource.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Audio_resource>&
get_audio_rsrc_data()
{
  static Nil::Data::Generic_data<Nil::Data::Audio_resource> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Audio_resource **data)
{
  *count = get_audio_rsrc_data().data.size();
  *data = get_audio_rsrc_data().data.data();
}


void
get(const Node &node, Audio_resource &out)
{
  get_audio_rsrc_data().get_data(node, out);
}


void
set(Node &node, const Audio_resource &in)
{
  get_audio_rsrc_data().set_data(node, in);
}


void
remove_audio_resource(Node &node)
{
  get_audio_rsrc_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_audio_resource(const Node &node)
{
  return get_audio_rsrc_data().find(node);
}


bool
has(const Node &node, const Audio_resource &)
{
  return has_audio_resource(node);
}


uint64_t
get_type_id(const Audio_resource &)
{
  return get_audio_rsrc_data().type_id;
}


const char*
get_type_name(const Audio_resource &data)
{
  return "Audio_resource";
}


size_t
audio_resource_count()
{
  return get_audio_rsrc_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Audio_resource **out_data, Node **out_node)
{
  return get_audio_rsrc_data().events(event, count, out_data, out_node);
}



} // ns
} // ns
