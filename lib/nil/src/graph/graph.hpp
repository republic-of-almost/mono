#ifndef GRAPH_INCLUDED_7A457725_B9B4_48E4_AA9E_0C3C587B7247
#define GRAPH_INCLUDED_7A457725_B9B4_48E4_AA9E_0C3C587B7247


#include "graph_fwd.hpp"
#include <lib/utilities.hpp>
#include <math/math.hpp>
#include <stdint.h>
#include <stddef.h>


namespace Nil {
namespace Graph {


// ------------------------------------------------------------ [ Life time ] --


void
initialize(Data *data);


void
think(Data *data);


void
destroy(Data *data);


uint64_t
last_tick(Data *data);


// ---------------------------------------------------------------- [ Nodes ] --


bool
node_exists(
  const Data *data,
  const uint32_t node_id,
  size_t *out_index = nullptr);
  
bool
node_pending(
  const Data *data,
  const uint32_t node_id,
  size_t *out_index = nullptr);


uint32_t
node_create(Data *data);


bool
node_remove(
  Data *data,
  const uint32_t node_id);


bool
node_set_parent(
  Data *data,
  const uint32_t parent_id,
  const uint32_t child_id);


size_t
node_child_count(
  const Data *data,
  const uint32_t node_id);


size_t
node_descendants_count(
  const Data *data,
  const uint32_t node_id);


uint32_t
node_get_child(
  const Data *data,
  const uint32_t node_id,
  const size_t child_index);


uint32_t
node_get_parent(const Data *data, const uint32_t node_id);


bool
node_modified(Data *data, const uint32_t node_id);


// ----------------------------------------------------------- [ Attributes ] --


bool
node_get_name(
  const Data *graph,
  const uint32_t node_id,
  const char **name);

  
bool
node_set_name(
  Data *graph,
  const uint32_t node_id,
  const char *name);


bool
node_get_transform(
  const Data *data,
  const uint32_t node_id,
  math::transform *trans,
  const bool world = false);


bool
node_set_transform(
  Data *data,
  const uint32_t node_id,
  const math::transform *trans);


bool
node_get_bounding_box(
  const Data *data,
  const uint32_t node_id,
  math::aabb *aabb);


bool
node_set_bounding_box(
  Data *data,
  const uint32_t node_id,
  const math::aabb *aabb);


bool
node_get_data_type_id(
  const Data *data,
  const uint32_t node_id,
  uint64_t *type_id);


bool
node_set_data_type_id(
  Data *data,
  const uint32_t node_id,
  const uint64_t *type_id);


bool
node_get_user_data(
  const Data *data,
  const uint32_t node_id,
  uintptr_t *user_data);


bool
node_set_user_data(
  Data *data,
  const uint32_t node_id,
  const uintptr_t *user_data);


} // ns
} // ns


#endif // inc guard
