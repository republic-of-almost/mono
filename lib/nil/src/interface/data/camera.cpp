#include <nil/data/camera.hpp>
#include <nil/data/transform.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <math/transform/transform.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Camera>&
get_camera_data()
{
  static Nil::Data::Generic_data<Nil::Data::Camera> data(
    Nil::Data::get_type_id(Nil::Data::Transform{}),
    [](uint32_t id, uintptr_t user_data, size_t index)
    {
      Nil::Data::Generic_data<Nil::Data::Camera> *data = reinterpret_cast<Nil::Data::Generic_data<Nil::Data::Camera>*>(user_data);
      LIB_ASSERT(data);
      
      Nil::Node node(id);
      
      Nil::Data::Transform trans;
      Nil::Data::get(node, trans, true);

      math::transform internal_trans = math::transform_init(
      math::vec3_init(trans.position),
      math::vec3_init(trans.scale),
      math::quat_init(trans.rotation[0], trans.rotation[1], trans.rotation[2], trans.rotation[3])
      );

      math::mat4 view = transform_get_lookat_matrix(internal_trans, math::vec3_init(0,0,1), math::vec3_init(0,1,0));

      memcpy(
        data->data[index].view_mat,
        math::mat4_get_data(view),
        sizeof(data->data[index].view_mat)
      );
      
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
get(size_t *count, Camera **cameras)
{
  *cameras = get_camera_data().data.begin();
  *count   = get_camera_data().data.size();
}


void
get(const Node &node, Camera &out)
{
  get_camera_data().get_data(node, out);
}


void
set(Node &node, const Camera &in)
{
  Camera cpy = in;

  Nil::Data::Transform trans;
  Nil::Data::get(node, trans, true);

  math::transform internal_trans = math::transform_init(
  math::vec3_init(trans.position),
  math::vec3_init(trans.scale),
  math::quat_init(trans.rotation[0], trans.rotation[1], trans.rotation[2], trans.rotation[3])
  );

  math::mat4 view = transform_get_lookat_matrix(internal_trans, math::vec3_init(1,0,0), math::vec3_init(0,1,0));

  memcpy(
    cpy.view_mat,
    math::mat4_get_data(view),
    sizeof(cpy.view_mat)
  );

  get_camera_data().set_data(node, cpy);
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


bool
has(const Node &node, const Camera &)
{
  return has_camera(node);
}


uint64_t
get_type_id(const Camera &)
{
  return get_camera_data().type_id;
}


const char*
get_type_name(const Camera &in)
{
  return "Camera";
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
