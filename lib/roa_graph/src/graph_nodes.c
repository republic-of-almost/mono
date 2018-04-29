#include <roa_graph/roa_graph.h>
#include <graph_data.h>
#include <graph_helpers.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_lib/log.h>
#include <graph_data.h>
#include <stdlib.h>


/* ----------------------------------------------------- [ Config / Data ] -- */


#define ROA_GRAPH_INTEGRITY_CHECK 1


/* ---------------------------------------------------- [ Error Messages ] -- */


const char msg_failed_to_find_node[] = "Failed to find node_id %d";
const char msg_invalid_params[]			 = "Invalid paramaters";


/* ------------------------------------------------------ [ Misc Helpers ] -- */


static void
graph_size_check(struct roa_graph_ctx * graph)
{
  if (ROA_IS_ENABLED(ROA_GRAPH_INTEGRITY_CHECK))
  {
    ROA_ASSERT(graph);

    unsigned roa_graph_node_count      = roa_array_size(graph->node_id);
    unsigned parent_depth_data_count   = roa_array_size(graph->parent_depth_data);
    unsigned local_transform_count     = roa_array_size(graph->local_transform);
    unsigned world_transform_count     = roa_array_size(graph->world_transform);
    unsigned roa_graph_node_data_count = roa_array_size(graph->data);

    ROA_ASSERT(roa_graph_node_count == parent_depth_data_count);
    ROA_ASSERT(roa_graph_node_count == local_transform_count);
    ROA_ASSERT(roa_graph_node_count == world_transform_count);
    ROA_ASSERT(roa_graph_node_count == roa_graph_node_data_count);
  }

  return;
}


/* ------------------------------------------------- [ Node Data Helpers ] -- */


static uint32_t
get_parent_id(const uint64_t data)
{
  return ROA_UPPER_32_BITS(data);
}


static uint32_t
get_depth(const uint64_t data)
{
  return ROA_LOWER_32_BITS(data);
}


static uint64_t
set_data(const uint32_t parent, const uint32_t depth)
{
  return ROA_PACK3232(depth, parent);
}


/* ------------------------------------------------------------- [ Nodes ] -- */


ROA_BOOL
roa_graph_node_exists(
  const roa_graph_ctx_t graph,
  const uint32_t node_id,
  unsigned *index)
{
  /* param check */
  ROA_ASSERT(graph);
  ROA_ASSERT(node_id);

  uint32_t *ids = roa_array_data(graph->node_id);
  uint32_t count = roa_array_size(graph->node_id);

  /* Search for key */
  return key_search(
    node_id,
    ids,
    count,
    index);
}


uint32_t
roa_graph_node_create(roa_graph_ctx_t graph)
{
  /* param check */
  ROA_ASSERT(graph);

  /* Create New Node */
  const uint32_t reserved_bits = 0;

  const uint32_t new_id = ROA_PACK824(
    reserved_bits,
    ++graph->instance_counter
  );

  roa_array_push(graph->node_id, new_id);
  roa_array_push(graph->parent_depth_data, 0);

  roa_transform default_transform;
  roa_transform_init(&default_transform);

  roa_array_push(graph->local_transform, default_transform);
  roa_array_push(graph->world_transform, default_transform);

  struct node_data data;
  data.node_type_id = 0;
  data.tags = 0;
  data.user_data = 0;

  roa_array_push(graph->data, data);

  if (ROA_IS_ENABLED(ROA_GRAPH_INTEGRITY_CHECK))
  {
    graph_size_check(graph);
  }

  return new_id;
}


ROA_BOOL
roa_graph_node_remove(roa_graph_ctx_t graph, const uint32_t node_id)
{
  /* param check */
  ROA_ASSERT(graph);
  ROA_ASSERT(node_id);

  /* Remove The Node */
  if (roa_graph_node_exists(graph, node_id, ROA_NULL))
  {
    /* Create Event */
    unsigned index = 0;

    if (roa_graph_node_exists(graph, node_id, &index))
    {
      unsigned dec_count = roa_graph_node_descendants_count(graph, node_id);
			unsigned remove_count = dec_count + 1;
			uint32_t i;

      for (i = 0; i < remove_count; ++i)
      {
        roa_array_erase(graph->node_id, index);
        roa_array_erase(graph->parent_depth_data, index);
        roa_array_erase(graph->local_transform, index);
        roa_array_erase(graph->world_transform, index);
        roa_array_erase(graph->data, index);

        if (ROA_IS_ENABLED(ROA_GRAPH_INTEGRITY_CHECK))
        {
          graph_size_check(graph);
        }
      }
    }

    return ROA_TRUE;
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_recalc_transform_branch(
  roa_graph_ctx_t graph,
  const uint32_t this_id)
{
  unsigned this_index = 0;
  const ROA_BOOL exists = roa_graph_node_exists(graph, this_id, &this_index);

  if (!exists)
  {
    ROA_LOG_FATAL("Graph corrupted");
    return ROA_FALSE;
  }

  uint32_t curr_depth = get_depth(this_index);
  roa_transform last_world_trans;
  roa_transform_init(&last_world_trans);

  const uint32_t parent_id = roa_graph_node_get_parent(graph, this_id);

  if (parent_id)
  {
    unsigned parent_index = 0;

    ROA_BOOL parent_exists = roa_graph_node_exists(
			graph,
			parent_id,
			&parent_index);

    if (!parent_exists)
    {
      ROA_LOG_FATAL("Graph corrupted");
      return ROA_FALSE;
    }

    last_world_trans = graph->world_transform[parent_index];
  }

  /* root transform */
  roa_transform *transform_stack = ROA_NULL;
  roa_array_create_with_capacity(transform_stack, 32);
  roa_array_push(transform_stack, last_world_trans);

  unsigned dec_count = roa_graph_node_descendants_count(graph, this_id);
  unsigned nodes_to_calc = dec_count + 1; 

	unsigned i;

  for (i = 0; i < nodes_to_calc; ++i)
  {
    const unsigned   index = this_index + i;
    const uint64_t data = graph->parent_depth_data[index];
    const uint32_t depth = get_depth(data);

    /* Pop off all unrequired transforms. */
    if (curr_depth > depth)
    {
      const unsigned to_pop = curr_depth - depth;

			unsigned j;

      for (j = 0; j < to_pop; ++j)
      {
        roa_array_pop(transform_stack);
      }
    }
    else if (curr_depth < depth)
    {
      roa_array_push(transform_stack, last_world_trans);
    }

    curr_depth = depth;

    const roa_transform local_transform = graph->local_transform[index];

    /* Calc new world transform. */
    roa_transform child_world;
    roa_transform transform_top = roa_array_back(transform_stack);
    roa_transform_inherited(&child_world, &transform_top, &local_transform);

    graph->world_transform[index] = child_world;
    last_world_trans = child_world;
  }

  roa_array_destroy(transform_stack);

  return ROA_TRUE;
}





ROA_BOOL
roa_graph_node_set_parent(
  roa_graph_ctx_t graph,
  const uint32_t this_id,
  const uint32_t parent_id)
{
  /* param check */
  ROA_ASSERT(graph);
  ROA_ASSERT(this_id);

  /* Find this entities details */
  unsigned this_index = 0;
  uint32_t this_depth = 0;
  {
    uint32_t *ids = graph->node_id;
    unsigned count = roa_array_size(graph->node_id);

    if (!key_search(
      this_id,
      ids,
      count,
      &this_index))
    {
      return ROA_FALSE;
    }

    this_depth = get_depth(graph->parent_depth_data[this_index]);
  }

  /* Find out how many nodes we need to move */
  uint32_t nodes_to_move = 1;
  {
    const unsigned roa_graph_node_count = roa_array_size(graph->node_id);
    const unsigned start_index = this_index + 1;

		unsigned i;

    for (i = start_index; i < roa_graph_node_count; ++i, ++nodes_to_move)
    {
      const uint32_t curr_depth = get_depth(graph->parent_depth_data[i]);

      if (curr_depth <= this_depth)
      {
        break;
      }
    }
  }

  /* Remove nodes, and insert else where in the tree */
  if(nodes_to_move)
  {
    /* Save the old data */
    uint32_t *move_nodes = ROA_NULL;
    {
      roa_array_create_with_capacity(move_nodes, nodes_to_move);
      roa_array_resize(move_nodes, nodes_to_move);

			unsigned move_bytes = sizeof(move_nodes[0]) * nodes_to_move;
      memcpy(move_nodes, &graph->node_id[this_index], move_bytes);

			unsigned i;

      for (i = 0; i < nodes_to_move; ++i)
      {
        roa_array_erase(graph->node_id, this_index);
      }
    }

    uint64_t *move_parent_depth_data = ROA_NULL;
    {
      roa_array_create_with_capacity(move_parent_depth_data, nodes_to_move);
      roa_array_resize(move_parent_depth_data, nodes_to_move);
      
			unsigned move_bytes = sizeof(move_parent_depth_data[0]) * nodes_to_move;
			memcpy(move_parent_depth_data, &graph->parent_depth_data[this_index], move_bytes);

			unsigned i;

      for (i = 0; i < nodes_to_move; ++i)
      {
        roa_array_erase(graph->parent_depth_data, this_index);
      }
    }

    roa_transform *move_local_transform = ROA_NULL;
    {
      roa_array_create_with_capacity(move_local_transform, nodes_to_move);
      roa_array_resize(move_local_transform, nodes_to_move);

			unsigned move_bytes = sizeof(move_local_transform[0]) * nodes_to_move;
      memcpy(move_local_transform, &graph->local_transform[this_index], move_bytes); 

			unsigned i;

      for (i = 0; i < nodes_to_move; ++i)
      {
        roa_array_erase(graph->local_transform, this_index);
      }
    }

    roa_transform *move_world_transform = ROA_NULL;
    {
      roa_array_create_with_capacity(move_world_transform, nodes_to_move);
      roa_array_resize(move_world_transform, nodes_to_move);

			unsigned move_bytes = sizeof(move_world_transform[0]) * nodes_to_move;
      memcpy(move_world_transform, &graph->world_transform[this_index], move_bytes);

			unsigned i;

      for (i = 0; i < nodes_to_move; ++i)
      {
        roa_array_erase(graph->world_transform, this_index);
      }
    }

    struct node_data *data_to_move = ROA_NULL;
    {
      roa_array_create_with_capacity(data_to_move, nodes_to_move);
      roa_array_resize(data_to_move, nodes_to_move);

			unsigned move_bytes = sizeof(data_to_move[0]) * nodes_to_move;
      memcpy(data_to_move, &graph->data[this_index], move_bytes); 

			unsigned i;

      for (i = 0; i < nodes_to_move; ++i)
      {
        roa_array_erase(graph->data, this_index);
      }
    }

    if (ROA_IS_ENABLED(ROA_GRAPH_INTEGRITY_CHECK))
    {
      graph_size_check(graph);
    }

    /* Find new insert point */
    unsigned parent_index = 0;
    unsigned insert_index = roa_array_size(graph->node_id);
    uint32_t parent_depth = 0;
    {
      if (parent_id > 0)
      {
        const uint32_t *node_ids = graph->node_id;
        const unsigned roa_graph_node_count = roa_array_size(graph->node_id);

        if (!key_search(
          parent_id,
          node_ids,
          roa_graph_node_count,
          &parent_index))
        {
          ROA_LOG_FATAL("Graph is corrupted");
          return ROA_FALSE;
        }

        insert_index = parent_index + 1;
        parent_depth = get_depth(graph->parent_depth_data[parent_index]);
      }
    }

    /* Change the first node to point at the new parent. */
    move_parent_depth_data[0] = set_data(parent_id, get_depth(move_parent_depth_data[0]));

    /*
    Insert the data into the new positions.
    */
    {
			unsigned i;

      for (i = 0; i < nodes_to_move; ++i)
      {
        roa_array_insert(graph->node_id, insert_index + i, move_nodes[i]);
        roa_array_insert(graph->parent_depth_data, insert_index + i, move_parent_depth_data[i]);
        roa_array_insert(graph->local_transform, insert_index + i, move_local_transform[i]);
        roa_array_insert(graph->world_transform, insert_index + i, move_world_transform[i]);
        roa_array_insert(graph->data, insert_index + i, data_to_move[i]);
      }

      if (ROA_IS_ENABLED(ROA_GRAPH_INTEGRITY_CHECK))
      {
        graph_size_check(graph);
      }
    }

    /*
    Calculate the depth differences between old and new parents.
    If parent_id = 0 aka root node, we need to add an extra -1 to get the
    correct depth.
    */
    const int32_t old_parent_depth = this_depth - 1;
    const int32_t parent_diff = parent_depth - old_parent_depth;
    const int32_t depth_diff = parent_diff - (parent_id ? 0 : 1);

    /* Update the depth data */
		uint32_t i;
    for (i = 0; i < nodes_to_move; ++i)
    {
      const uint64_t old_data = move_parent_depth_data[i];
      const uint32_t old_depth = get_depth(old_data);
      const uint32_t depth = old_depth + depth_diff;
      const uint64_t new_data = set_data(get_parent_id(old_data), depth);
      const unsigned   index = insert_index + i;

      graph->parent_depth_data[index] = new_data;
    }

    const ROA_BOOL update_transform = roa_graph_node_recalc_transform_branch(
      graph,
      this_id
    );

    return update_transform;
  }

  return ROA_FALSE;
}


unsigned
roa_graph_node_child_count(
  const roa_graph_ctx_t graph,
  const uint32_t node_id)
{
  /* param check */
  ROA_ASSERT(graph);

  /* Count Children */
  unsigned index = 0;

  if (node_id > 0)
  {
    uint32_t *ids = graph->node_id;
    unsigned count = roa_array_size(graph->node_id);

    if (!key_search(
      node_id,
      ids,
      count,
      &index))
    {
      ROA_LOG_ERROR(msg_failed_to_find_node, node_id);
      return 0;
    }
  }

  /* Calculate children. */
  unsigned child_count = 0;
  {
		uint64_t *pd_data = graph->parent_depth_data;
    int64_t this_depth = node_id ? (int64_t)get_depth(pd_data[index]) : -1;
    unsigned start_index = node_id ? index + 1 : 0;
    unsigned count = roa_array_size(graph->node_id);
		unsigned i;

    for (i = start_index; i < count; ++i)
    {
      const int64_t that_depth = get_depth(pd_data[i]);

      if (that_depth > this_depth)
      {
        if (that_depth == this_depth + 1)
        {
          ++child_count;
        }
      }
      else
      {
        break;
      }
    }
  }

  return child_count;
}


unsigned
roa_graph_node_descendants_count(
  const roa_graph_ctx_t graph,
  const uint32_t node_id)
{
  /* param check */
  ROA_ASSERT(graph);

  /* Count all decendants */
  unsigned child_count = 0;
  unsigned index = 0;

  if (node_id > 0)
  {
    uint32_t *ids = graph->node_id;
    unsigned count = roa_array_size(graph->node_id);

    if (!key_search(
      node_id,
      ids,
      count,
      &index))
    {
      ROA_LOG_ERROR(msg_failed_to_find_node, node_id);
      return 0;
    }
  }

  /* Calculate descendants */
  {
		uint64_t *pd_data   = graph->parent_depth_data;
    int32_t this_depth = node_id ? (int32_t)get_depth(pd_data[index]) : -1;
    unsigned start     = this_depth >= 0 ? index + 1 : 0;
    unsigned id_count  = roa_array_size(graph->node_id);
		unsigned i;

    for (i = start; i < id_count; ++i)
    {
      const int32_t that_depth = get_depth(pd_data[i]);

      if (that_depth > this_depth)
      {
        ++child_count;
      }
      else
      {
        break;
      }
    }
  }

  return child_count;
}


uint32_t
roa_graph_node_get_child(
  const roa_graph_ctx_t graph,
  const uint32_t node_id,
  const unsigned child_index)
{
  /* param check */
  ROA_ASSERT(graph);

  /* Count Children */
  unsigned child_count = 0;
  unsigned index = 0;

  if (node_id > 0)
  {
    uint32_t *ids = graph->node_id;
    unsigned id_count = roa_array_size(graph->node_id);

    if (!key_search(
      node_id,
      ids,
      id_count,
      &index))
    {
      ROA_LOG_ERROR(msg_failed_to_find_node, node_id);
      return 0;
    }
  }

  /* Calculate children. */
  {
		uint64_t *pd_data = graph->parent_depth_data;
    int64_t this_depth = node_id ? (int64_t)get_depth(pd_data[index]) : (int64_t)-1;
    unsigned start_index = node_id ? index + 1 : 0;
    unsigned id_count = roa_array_size(graph->node_id);
		unsigned i;

    for (i = start_index; i < id_count; ++i)
    {
      const int64_t that_depth = get_depth(pd_data[i]);

      if (that_depth > this_depth)
      {
        if (that_depth == this_depth + 1)
        {
          if (child_count == child_index)
          {
            return graph->node_id[i];
          }
          ++child_count;
        }
      }
      else
      {
        break;
      }
    }
  }

  return 0;
}


uint32_t
roa_graph_node_get_parent(const roa_graph_ctx_t graph, const uint32_t node_id)
{
  unsigned index = 0;

  if (roa_graph_node_exists(graph, node_id, &index))
  {
    return get_parent_id(graph->parent_depth_data[index]);
  }

  return 0;
}


/* -------------------------------------------------------- [ Attributes ] -- */


ROA_BOOL
roa_graph_node_get_name(
  const roa_graph_ctx_t data,
  const uint32_t node_id,
  const char **name)
{
  unsigned index = 0;

  if (roa_graph_node_exists(data, node_id, &index))
  {
    *name = data->data[index].name.data;
    return ROA_TRUE;
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_set_name(
  roa_graph_ctx_t data,
  const uint32_t node_id,
  const char *name)
{
  unsigned index = 0;

  const unsigned str_len = strlen(name) + 1;

  const unsigned name_size = sizeof(struct short_string) / sizeof(char);

  char clipped[ROA_GRAPH_SHORT_STRING];
  ROA_MEM_ZERO(clipped);

  memcpy(clipped, name, str_len > name_size ? name_size : str_len);

  if (roa_graph_node_exists(data, node_id, &index))
  {
    memcpy(data->data[index].name.data, clipped, str_len);
    return ROA_TRUE;
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_get_tags(
  const roa_graph_ctx_t graph,
  const uint32_t node_id,
  uint64_t *tags)
{
  /* param check */
  {
    ROA_ASSERT(graph);
    ROA_ASSERT(node_id);
    ROA_ASSERT(tags != ROA_NULL);

    if (!graph || !node_id || !tags)
    {
      ROA_LOG_ERROR(msg_invalid_params);
      return ROA_FALSE;
    }
  }

  /* Get the Current Tags */
  {
    unsigned index = 0;

    if (roa_graph_node_exists(graph, node_id, &index))
    {
      *tags = graph->data[index].tags;
      return ROA_TRUE;
    }
  }

  ROA_LOG_ERROR(msg_failed_to_find_node);
  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_set_tags(
  roa_graph_ctx_t graph,
  const uint32_t node_id,
  const uint64_t tags)
{
  /* param check */
  {
    ROA_ASSERT(graph);
    ROA_ASSERT(node_id);

    if (!graph || !node_id)
    {
      ROA_LOG_ERROR(msg_invalid_params);
      return ROA_FALSE;
    }
  }

  /* Set the Current Tags */
  {
    unsigned index = 0;

    if (roa_graph_node_exists(graph, node_id, &index))
    {
      graph->data[index].tags = tags;
      return ROA_TRUE;
    }
  }

  ROA_LOG_ERROR(msg_failed_to_find_node, node_id);
  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_get_transform(
  const roa_graph_ctx_t data,
  const uint32_t node_id,
  roa_transform *trans,
  const ROA_BOOL inherited)
{
  unsigned index = 0;

  if (roa_graph_node_exists(data, node_id, &index))
  {
    if (!inherited)
    {
      *trans = data->local_transform[index];
    }
    else
    {
      *trans = data->world_transform[index];
    }
    return ROA_TRUE;
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_set_transform(
  roa_graph_ctx_t graph,
  const uint32_t node_id,
  const roa_transform *trans)
{
  unsigned index = 0;
  const ROA_BOOL found = roa_graph_node_exists(graph, node_id, &index);

  if (found)
  {
    graph->local_transform[index] = *trans;

    return roa_graph_node_recalc_transform_branch(
      graph,
      node_id
    );
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_register_type(
  roa_graph_ctx_t data,
  const uint32_t node_id,
  const uint64_t type_id)
{
  unsigned index = 0;

  if (roa_graph_node_exists(data, node_id, &index))
  {
    data->data[index].node_type_id |= type_id;

    return ROA_TRUE;
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_unregister_type(
  roa_graph_ctx_t data,
  const uint32_t node_id,
  const uint64_t type_id)
{
  unsigned index = 0;

  if (roa_graph_node_exists(data, node_id, &index))
  {
    data->data[index].node_type_id &= ~type_id;

    return ROA_TRUE;
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_get_data_type_id(
  const roa_graph_ctx_t data,
  const uint32_t node_id,
  uint64_t *type_id)
{
  unsigned index = 0;

  if (roa_graph_node_exists(data, node_id, &index))
  {
    *type_id = data->data[index].node_type_id;
    return ROA_TRUE;
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_get_user_data(
  const roa_graph_ctx_t data,
  const uint32_t node_id,
  void **user_data)
{
  unsigned index = 0;

  if (roa_graph_node_exists(data, node_id, &index))
  {
    *user_data = data->data[index].user_data;
    return ROA_TRUE;
  }

  return ROA_FALSE;
}


ROA_BOOL
roa_graph_node_set_user_data(
  roa_graph_ctx_t data,
  const uint32_t node_id,
  void *user_data)
{
  unsigned index = 0;

  if (roa_graph_node_exists(data, node_id, &index))
  {
    data->data[index].user_data = user_data;
    return ROA_TRUE;
  }

  return ROA_FALSE;
}



/* ------------------------------------------------------------ [ Config ] -- */


#undef ROA_GRAPH_INTEGRITY_CHECK
