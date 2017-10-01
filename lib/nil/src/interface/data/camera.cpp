#include <nil/data/camera.hpp>
#include <nil/data/transform.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
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
      Nil::Data::get(node, trans);

      const math::transform internal_trans = math::transform_init(
        math::vec3_init(trans.world_position),
        math::vec3_init(trans.world_scale),
        math::quat_init(trans.world_rotation)
      );

      const math::mat4 view = transform_lookat_matrix(internal_trans);

      memcpy(
        data->data[index].view_mat,
        math::mat4_data(view),
        sizeof(data->data[index].view_mat)
      );
      
      memcpy(
        data->data[index].position,
        trans.world_position,
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
  Nil::Data::get(node, trans);

  math::transform internal_trans = math::transform_init(
  math::vec3_init(trans.world_position),
  math::vec3_init(trans.world_scale),
  math::quat_init(trans.world_rotation[0], trans.world_rotation[1], trans.world_rotation[2], trans.world_rotation[3])
  );

  math::mat4 view = transform_lookat_matrix(internal_trans);

  memcpy(
    cpy.view_mat,
    math::mat4_data(view),
    sizeof(cpy.view_mat)
  );

  get_camera_data().set_data(node, cpy);
  
  // -- Resort in order of priority -- //
  {
    const size_t count = get_camera_data().keys.size();
    
    Nil::Data::Camera *cam = get_camera_data().data.data();
    uint32_t *keys = get_camera_data().keys.data();
  
    for(int i = 0; i < count; ++i)
    {
      for(int j = i; j < count; ++j)
      {
        const int32_t priority_a = cam[i].priority;
        const int32_t priority_b = cam[j].priority;
      
        if(priority_b < priority_a)
        {
          const uint32_t key_temp          = keys[i];
          const Nil::Data::Camera cam_temp = cam[i];
          
          keys[i] = keys[j];
          cam[i] = cam[j];
          
          keys[j] = key_temp;
          cam[j] = cam_temp;
        }
      }
    }
  }

  
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
  return find_node(
    node,
    get_camera_data().keys.data(),
    get_camera_data().keys.size()
  );

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
