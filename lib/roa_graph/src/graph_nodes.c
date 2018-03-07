#include <roa_graph/roa_graph.h>
#include <roa_lib/atomic.h>
#include <roa_lib/spin_lock.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/array.h>
#include <graph_data.h>


uint32_t
roa_graph_node_create(roa_graph_ctx_t ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  uint32_t new_node_id = roa_atomic_int_inc(&ctx->object_counter);
  struct roa_graph_object *obj = roa_zalloc(sizeof(*obj));

  /* create new object */
  if(obj && new_node_id)
  {
    /* create new object */
    roa_spin_lock_init(&obj->object_lock);

    roa_array_create(obj->object_ids, 8);
    roa_array_create(obj->parent_ids, 8);
    roa_array_create(obj->local_transform, 8);
    roa_array_create(obj->world_transform, 8);

    /* insert */
    roa_spin_lock_aquire(&ctx->objects_lock);

    roa_array_push(ctx->object_ids, new_node_id);
    roa_array_push(ctx->objects, obj);

    roa_spin_lock_release(&ctx->objects_lock);
  }

  return new_node_id && obj ? new_node_id : 0;
}


ROA_BOOL
roa_graph_node_set_parent(roa_graph_ctx_t ctx, uint32_t node, uint32_t parent_node)
{
  return ROA_FALSE;
}


uint32_t
roa_graph_node_get_parent(roa_graph_ctx_t ctx, uint32_t node)
{
  return 0;
}


ROA_BOOL
roa_graph_node_remove(roa_graph_ctx_t ctx, uint32_t node_id)
{
  ROA_BOOL result = ROA_FALSE;

  roa_spin_lock_aquire(&ctx->objects_lock);

  unsigned count = roa_array_size(ctx->object_ids, object_ids);

  for (unsigned i = 0; i < count; ++i)
  {
    if (ctx->object_ids[i] == node_id)
    {
      /* get the object and free */
      struct roa_graph_object *obj = ctx->objects[i];

      roa_spin_lock_aquire(&obj->object_lock);
      
      /* free resources */
      roa_array_destroy(obj->object_ids);
      roa_array_destroy(obj->parent_ids);
      roa_array_destroy(obj->local_transform);
      roa_array_destroy(obj->world_transform);

      /* remove from graph */
      roa_array_erase(ctx->object_ids, i);
      roa_array_erase(ctx->objects, i);

      roa_spin_lock_release(&obj->object_lock);

      roa_free(obj);

      result = ROA_TRUE;
    }
  }

  roa_spin_lock_release(&ctx->objects_lock);

  return result;
}


unsigned
roa_graph_node_count(roa_graph_ctx_t ctx)
{
  unsigned count = 0;

  roa_spin_lock_aquire(&ctx->objects_lock);

  count = roa_array_size(ctx->object_ids, object_ids);

  roa_spin_lock_release(&ctx->objects_lock);

  return count;
}