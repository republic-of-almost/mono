#include <nil/data/light.hpp>
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
Nil::Data::Generic_data<Nil::Data::Light>&
get_light_data()
{
  static Nil::Data::Generic_data<Nil::Data::Light> data(
    Nil::Data::get_type_id(Nil::Data::Transform{}),
    [](uint32_t id, uintptr_t user_data, size_t index)
    {
      Nil::Data::Generic_data<Nil::Data::Light> *data = reinterpret_cast<Nil::Data::Generic_data<Nil::Data::Light>*>(user_data);
      LIB_ASSERT(data);
      
      Nil::Node node(id);
      
      Nil::Data::Transform trans;
      Nil::Data::get(node, trans, true);

      memcpy(
        data->data[index].position,
        trans.position,
        sizeof(data->data[index].position)
      );
    }
  );
  
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, Light **out)
{
  *count = get_light_data().data.size();
  *out = get_light_data().data.data();
}


void
get(const Node &node, Light &out)
{
  get_light_data().get_data(node, out);
}


void
set(Node &node, const Light &in)
{
  Light cpy = in;

  Nil::Data::Transform trans;
  Nil::Data::get(node, trans, true);

  memcpy(
    cpy.position,
    trans.position,
    sizeof(in.position)
  );

  get_light_data().set_data(node, cpy);
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
