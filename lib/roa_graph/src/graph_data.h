#ifndef GRAPH_DATA_INCLUDED_C75347EC_83FC_47B2_89DF_8F641A0DD327
#define GRAPH_DATA_INCLUDED_C75347EC_83FC_47B2_89DF_8F641A0DD327


#include <roa_lib/fundamental.h>
#include <roa_math/math.h>
#include <roa_lib/atomic.h>


struct roa_graph_object
{
  roa_atomic_int object_lock;

  uint32_t *object_ids;
  uint32_t *parent_ids;

  roa_transform *local_transform;
  roa_transform *world_transform;
};


struct roa_graph_ctx
{
  roa_atomic_int objects_lock;

  uint32_t *object_ids;
  struct roa_graph_object **objects;

  roa_atomic_int object_counter;
};


#endif /* inc guard */