#include <nil/data/audio.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Audio>&
get_audio_data()
{
  static Nil::Data::Generic_data<Nil::Data::Audio> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Audio **data)
{
  *count = get_audio_data().data.size();
  *data = get_audio_data().data.data();
}


void
get(const Node &node, Audio &out)
{
  get_data(
    node,
    out,
    get_audio_data().data.data(),
    get_audio_data().keys.data(),
    get_audio_data().keys.size()
  );
}


void
set(Node &node, const Audio &in)
{
  // get_audio_data().set_data(node, in);
  size_t out_index = 0;

  if(find_node(
    node,
    get_audio_data().keys.data(),
    get_audio_data().keys.size(),
    &out_index
  ))
  {
    get_audio_data().data[out_index] = in;
    get_audio_data().actions[out_index] |= Nil::Data::Event::UPDATED;
  }
  else
  {
    // Graph add type_id info
  }
  
  Graph::data_updated(
    Nil::Data::get_graph_data(),
    node.get_id(),
    get_audio_data().type_id
  );
}


void
remove_audio(Node &node)
{
  get_audio_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_audio(const Node &node)
{
  return find_node(
    node,
    get_audio_data().keys.data(),
    get_audio_data().keys.size()
  );
}


bool
has(const Node &node, const Audio &)
{
  return has_audio(node);
}


uint64_t
get_type_id(const Audio &)
{
  return get_audio_data().type_id;
}


const char*
get_type_name(const Audio &)
{
  return "Audio";
}


size_t
audio_count()
{
  return get_audio_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Audio **out_data, Node **out_node)
{
  return get_audio_data().events(event, count, out_data, out_node);
}


} // ns
} // ns
