#include <nil/data/camera.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Camera>&
get_camera_data()
{
  static Nil::Data::Generic_data<Nil::Data::Camera> data;
  return data;
}


} // ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(const Node &node, Camera &out)
{
  get_camera_data().get_data(node, out);
}


void
set(Node &node, const Camera &in)
{
  get_camera_data().set_data(node, in);
}


void
remove_camera(Node &node)
{
  get_camera_data().remove_data(node);
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_camera(const Node &node)
{
  return get_camera_data().find(node);
}


uint64_t
get_type_id(const Camera &)
{
  NIL_DATA_TYPE_ID_REG
}


size_t
camera_count()
{
  return get_camera_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Camera **out_data, Node **out_node)
{
  return get_camera_data().events(event, count, out_data, out_node);
}



} // ns
} // ns
