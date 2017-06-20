#include <nil/data/renderable.hpp>
#include <nil/data/transform.hpp>
#include <nil/node.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"

#include <stdint.h>
#include <stddef.h>


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Renderable>&
get_mat_data()
{
  static Nil::Data::Generic_data<Nil::Data::Renderable> data(
    Nil::Data::get_type_id(Nil::Data::Transform{}),
    [](uint32_t id, uintptr_t user_data, size_t index)
    {
      Nil::Data::Generic_data<Nil::Data::Renderable> *data = reinterpret_cast<Nil::Data::Generic_data<Nil::Data::Renderable>*>(user_data);
      LIB_ASSERT(data);
    
      Nil::Node node(id);
      
      Nil::Data::Transform trans;
      Nil::Data::get(node, trans, true);

      math::transform internal_trans = math::transform_init(
      math::vec3_init_with_array(trans.position),
      math::vec3_init_with_array(trans.scale),
      math::quat_init(trans.rotation[0], trans.rotation[1], trans.rotation[2], trans.rotation[3])
      );

      math::mat4 mat = math::transform_get_world_matrix(internal_trans);

      memcpy(
        data->data[index].world_mat,
        math::mat4_get_data(mat),
        sizeof(data->data[index].world_mat)
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
get(size_t *count, Renderable **renderables)
{
  *renderables = get_mat_data().data.begin();
  *count       = get_mat_data().data.size();
}


void
get(const Node &node, Renderable &out)
{
  get_mat_data().get_data(node, out);
}


void
set(Node &node, const Renderable &in)
{
  Renderable cpy = in;

  Nil::Data::Transform trans;
  Nil::Data::get(node, trans, true);

  math::transform internal_trans = math::transform_init(
  math::vec3_init_with_array(trans.position),
  math::vec3_init_with_array(trans.scale),
  math::quat_init(trans.rotation[0], trans.rotation[1], trans.rotation[2], trans.rotation[3])
  );

  math::mat4 mat = math::transform_get_world_matrix(internal_trans);

  memcpy(
    cpy.world_mat,
    math::mat4_get_data(mat),
    sizeof(cpy.world_mat)
  );

  get_mat_data().set_data(node, cpy);
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
