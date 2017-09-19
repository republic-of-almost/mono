#include <nil/data/renderable_text.hpp>
#include <nil/data/transform.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph_data.hpp>
#include <graph/graph.hpp>
#include <math/transform/transform.hpp>
#include <lib/utilities.hpp>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


// -- Lazy Getter -- //
Nil::Data::Generic_data<Nil::Data::Renderable_text>&
get_mat_data()
{
  static Nil::Data::Generic_data<Nil::Data::Renderable_text> data(
    Nil::Data::get_type_id(Nil::Data::Transform{}),
    [](uint32_t id, uintptr_t user_data, size_t index)
    {
      Nil::Data::Generic_data<Nil::Data::Renderable_text> *data = reinterpret_cast<Nil::Data::Generic_data<Nil::Data::Renderable_text>*>(user_data);
      LIB_ASSERT(data);
    
      Nil::Node node(id);
      
      Nil::Data::Transform trans;
      Nil::Data::get(node, trans);

      math::transform internal_trans = math::transform_init(
      math::vec3_init(trans.world_position),
      math::vec3_init(trans.world_scale),
      math::quat_init(trans.world_rotation[0], trans.world_rotation[1], trans.world_rotation[2], trans.world_rotation[3])
      );

      math::mat4 mat = math::transform_world_matrix(internal_trans);

      memcpy(
        data->data[index].world_mat,
        math::mat4_data(mat),
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
get(size_t *count, Renderable_text **renderables)
{
  get_mat_data().get_access(count, renderables);
}


void
get(const Node &node, Renderable_text &out)
{
  get_mat_data().get_data(node, out);
}


void
set(Node &node, const Renderable_text &in)
{
  Renderable_text cpy = in;
  
  Nil::Data::Transform trans;
  Nil::Data::get(node, trans);

  math::transform internal_trans = math::transform_init(
  math::vec3_init(trans.world_position),
  math::vec3_init(trans.world_scale),
  math::quat_init(trans.world_rotation[0], trans.world_rotation[1], trans.world_rotation[2], trans.world_rotation[3])
  );

  math::mat4 mat = math::transform_world_matrix(internal_trans);

  memcpy(
    cpy.world_mat,
    math::mat4_data(mat),
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
has_renderable_text(const Node &node)
{
  return find_node(
    node,
    get_mat_data().keys.data(),
    get_mat_data().keys.size()
  );

}


bool
has(const Node &node, const Renderable_text &data)
{
  return has_renderable_text(node);
}


uint64_t
get_type_id(const Renderable_text &)
{
  return get_mat_data().type_id;
}


const char*
get_type_name(const Renderable_text &in)
{
  return "Renderable Text";
}


size_t
renderable_text_count()
{
  return get_mat_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t event, size_t *count, Renderable_text **out_data, Node **out_node)
{
  return get_mat_data().events(event, count, out_data, out_node);
}



} // ns
} // ns
