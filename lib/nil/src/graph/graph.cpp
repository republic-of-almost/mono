#include "graph.hpp"
#include "graph_data.hpp"
#include <math/transform/transform.hpp>
#include <math/geometry/aabb.hpp>
#include <math/to_string.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <lib/bits.hpp>
#include <lib/key.hpp>
#include <lib/entity.hpp>
#include <lib/string.hpp>

namespace Nil {
namespace Graph {


namespace
{
  // Used to stack alloc transform stack //
  constexpr size_t stack_hint = 32;
  
  
  // ------------------------------------------------------- [ Misc Helpers ] --
  

  void
  graph_size_check(const Nil::Graph::Data *graph)
  {
    #ifndef NDEBUG
    LIB_ASSERT(graph);

    LIB_ASSERT(graph->node_id.size() == graph->parent_depth_data.size());
    LIB_ASSERT(graph->node_id.size() == graph->local_transform.size());
    LIB_ASSERT(graph->node_id.size() == graph->world_transform.size());
    LIB_ASSERT(graph->node_id.size() == graph->data.size());
    
    #else
      return;
    #endif
  }


  // -------------------------------------------------- [ Node Data Helpers ] --


  inline uint32_t
  get_parent_id(const uint64_t data)
  {
    return lib::bits::upper32(data);
  }


  inline uint32_t
  get_depth(const uint64_t data)
  {
    return lib::bits::lower32(data);
  }


  inline uint64_t
  set_data(const uint32_t parent, const uint32_t depth)
  {
    return lib::bits::pack3232(depth, parent);
  }

} // anon ns


// ------------------------------------------------------------ [ Life time ] --


void
initialize(Data *graph)
{
  graph->instance_counter = 0;
}


void
think(Data *graph)
{
  const size_t count = graph->graph_type_data.size();
  
  for(size_t i = 0; i < count; ++i)
  {
    graph_tick_fn tick_fn = graph->graph_type_data[i].tick_cb;
  
    if(tick_fn)
    {
      uintptr_t user_data = graph->graph_type_data[i].user_data;
    
      tick_fn(user_data);
    }
  }
}


void
destroy(Data *graph)
{
  
}


// ----------------------------------------------------------- [ Graph Data ] --


uint64_t
data_register_type(
  Data *graph,
  const graph_tick_fn &tick_cb,
  const node_delete_fn &delete_cb,
  const data_dependecy_alert_fn &dependency_cb,
  uintptr_t user_data,
  uint64_t dependency_id
)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  
  graph->graph_type_data.emplace_back(
    tick_cb,
    delete_cb,
    dependency_cb,
    user_data,
    uint64_t{1} << graph->graph_type_data.size(),
    dependency_id
  );
  
  return graph->graph_type_data.back().type_id;
}


bool
data_unregister_type(
  Data *data,
  uint64_t type_id
)
{
  // -- Param Check -- //
  LIB_ASSERT(data);
  
  const size_t count = data->graph_type_data.size();
  
  for(size_t i = 0; i < count; ++i)
  {
    if(data->graph_type_data[i].type_id == type_id)
    {
      data->graph_type_data.erase(i);
      return true;
    }
  }
  
  return false;
}


void
data_updated(const Data *graph, const uint32_t node_id, const uint64_t type_id)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  
  const size_t type_count = graph->graph_type_data.size();
  
  lib::array<data_dependecy_alert_fn, stack_hint> updated_nodes;
  lib::array<uintptr_t, stack_hint> updated_user_data;
  
  for(size_t i = 0; i < type_count; ++i)
  {
    const uint64_t dependencies = graph->graph_type_data[i].dependency;
  
    if(dependencies & type_id)
    {
      data_dependecy_alert_fn alert_fn = graph->graph_type_data[i].dependency_cb;
    
      if(alert_fn)
      {
        updated_nodes.emplace_back(alert_fn);
        updated_user_data.emplace_back(graph->graph_type_data[i].user_data);
      }
    }
  }
  
  if(updated_nodes.empty())
  {
    return;
  }
  
  const size_t decendents_count = Graph::node_descendants_count(graph, node_id) + 1;
  
  size_t index = 0;
  lib::key::linear_search(node_id, graph->node_id.data(), graph->node_id.size(), &index);
  
  for(size_t i = 0; i < decendents_count; ++i)
  {
    const size_t node_count = updated_nodes.size();
    
    for(size_t j = 0; j < node_count; ++j)
    {
      updated_nodes[j](graph->node_id[index + i], updated_user_data[j]);
    }
  }
}


// ---------------------------------------------------------------- [ Nodes ] --


bool
node_exists(
  const Data *graph,
  const uint32_t node_id,
  size_t *index)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node_id);

  // -- Search for key -- //
  return lib::key::linear_search(
    node_id,
    graph->node_id.data(),
    graph->node_id.size(),
    index);
}


uint32_t
node_create(Data *graph)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);

  // -- Create New Node -- //
  constexpr uint32_t reserved_bits = 0;
  
  const uint32_t new_id = lib::entity::create(
    reserved_bits,
    ++graph->instance_counter
  );
  
  graph->node_id.emplace_back(
    new_id
  );
  graph->parent_depth_data.emplace_back(
    uint64_t{0}
  );
  graph->local_transform.emplace_back(
    math::transform_init()
  );
  graph->world_transform.emplace_back(
    math::transform_init()
  );

  constexpr short_string default_name {"Node"};
  
  graph->data.emplace_back(
    uintptr_t{0},
    uint64_t{0},
    default_name
  );

  #ifndef NDEBUG
  graph_size_check(graph);
  #endif

  return new_id;
}


bool
node_remove(Data *graph, const uint32_t node_id)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node_id);
  
  // -- Remove The Node -- //
  if(node_exists(graph, node_id))
  {
    // -- Create Event -- //
    size_t index = 0;

    if(node_exists(graph, node_id, &index))
    {
      const size_t decendent_count = node_descendants_count(graph, node_id) + 1;
      
      for(uint32_t i = 0; i < decendent_count; ++i)
      {
        const uint32_t this_id = graph->node_id[index];
        const uint64_t type_id = graph->data[index].node_type_id;
      
        graph->node_id.erase(index);
        graph->parent_depth_data.erase(index);
        graph->local_transform.erase(index);
        graph->world_transform.erase(index);
        graph->data.erase(index);

        #ifndef NDEBUG
        graph_size_check(graph);
        #endif
        
        // -- Remove Other Data -- //
        for(size_t j = 0; j < graph->graph_type_data.size(); ++j)
        {
          if(graph->graph_type_data[j].type_id & type_id)
          {
            if(graph->graph_type_data[j].delete_cb)
            {
              graph->graph_type_data[j].delete_cb(
                this_id,
                graph->graph_type_data[j].user_data
              );
            }
          }
        } // for
      }
    }
  
    return true;
  }

  return false;
}


namespace {


/*
  Recalculates the transforms and bounding boxes of a branch that has been
  updated.
*/
bool
node_recalc_transform_branch(
  Data *graph,
  const uint32_t this_id)
{
  size_t this_index = 0;
  const bool exists = node_exists(graph, this_id, &this_index);
  
  if(!exists)
  {
    LOG_FATAL("Graph corrupted");
    return false;
  }

  uint32_t curr_depth = get_depth(this_index);
  math::transform last_world_trans = math::transform_init();
  
  const uint32_t parent_id = node_get_parent(graph, this_id);
  
  if(parent_id)
  {
    size_t parent_index = 0;
    
    bool parent_exists = node_exists(graph, parent_id, &parent_index);
    
    if(!parent_exists)
    {
      LOG_FATAL("Graph corrupted");
      return false;
    }
    
    last_world_trans = graph->world_transform[parent_index];
  }
  
  lib::array<math::transform, stack_hint> transform_stack;
  transform_stack.emplace_back(last_world_trans); // Root transform.
  
  const size_t nodes_to_calc = node_descendants_count(graph, this_id) + 1;
  
  for(uint32_t i = 0; i < nodes_to_calc; ++i)
  {
    const size_t   index = this_index + i;
    const uint64_t data  = graph->parent_depth_data[index];
    const uint32_t depth = get_depth(data);
    
    // Pop off all unrequired transforms.
    if(curr_depth > depth)
    {
      transform_stack.pop_back();
    }
    else if(curr_depth < depth)
    {
      transform_stack.emplace_back(last_world_trans);
    }
    
    curr_depth = depth;
    
    const math::transform local_transform = graph->local_transform[index];
  
    // Calc new world transform.
    const math::transform child_world(
      math::transform_inherited(
        transform_stack.top(),
        local_transform
      )
    );

    graph->world_transform[index] = child_world;
    last_world_trans = child_world;
  }
  
  return true;
}


} // anon ns


bool
node_set_parent(
  Data *graph,
  const uint32_t parent_id,
  const uint32_t this_id)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(this_id);

  // -- Find this entities details -- //
  size_t this_index = 0;
  uint32_t this_depth = 0;
  {
    if(!lib::key::linear_search(
      this_id,
      graph->node_id.data(),
      graph->node_id.size(),
      &this_index))
    {
      return false;
    }

    this_depth = get_depth(graph->parent_depth_data[this_index]);
  }

  // -- Find out how many nodes we need to move -- //
  uint32_t nodes_to_move = 1;
  {
    const size_t node_count = graph->node_id.size();
    const size_t start_index = this_index + 1;

    for(size_t i = start_index; i < node_count; ++i, ++nodes_to_move)
    {
      const uint32_t curr_depth = get_depth(graph->parent_depth_data[i]);
    
      if(curr_depth <= this_depth)
      {
        break;
      }
    }
  }

  // -- Remove nodes, and insert else where in the tree -- //
  {
    // Save the old data
    lib::array<uint32_t, stack_hint> move_nodes(
      graph->node_id.begin() + this_index,
      graph->node_id.begin() + (this_index + nodes_to_move)
    );
    graph->node_id.erase(this_index, nodes_to_move);
    
    lib::array<uint64_t, stack_hint> move_parent_depth_data(
      graph->parent_depth_data.begin() + this_index,
      graph->parent_depth_data.begin() + (this_index + nodes_to_move)
    );
    graph->parent_depth_data.erase(this_index, nodes_to_move);
    
    lib::array<math::transform, stack_hint> move_local_transform(
      graph->local_transform.begin() + this_index,
      graph->local_transform.begin() + (this_index + nodes_to_move)
    );
    graph->local_transform.erase(this_index, nodes_to_move);
    
    lib::array<math::transform, stack_hint> move_world_transform(
      graph->world_transform.begin() + this_index,
      graph->world_transform.begin() + (this_index + nodes_to_move)
    );
    graph->world_transform.erase(this_index, nodes_to_move);
    
    lib::array<node_data, stack_hint> data_to_move(
      graph->data.begin() + this_index,
      graph->data.begin() + (this_index + nodes_to_move)
    );
    graph->data.erase(this_index, nodes_to_move);

    #ifndef NDEBUG
    graph_size_check(graph);
    #endif
    
    // Find new insert point
    size_t parent_index   = 0;
    size_t insert_index   = graph->node_id.size();
    uint32_t parent_depth = 0;
    {
      if(parent_id > 0)
      {
        const uint32_t *node_ids = graph->node_id.data();
        const size_t node_count = graph->node_id.size();
        
        if(!lib::key::linear_search(
          parent_id,
          node_ids,
          node_count,
          &parent_index))
        {
          LOG_FATAL("Graph is corrupted");
          return false;
        }
        
        insert_index = parent_index + 1;
        parent_depth = get_depth(graph->parent_depth_data[parent_index]);
      }
    }
    
    // Change the first node to point at the new parent.
    move_parent_depth_data[0] = set_data(parent_id, get_depth(move_parent_depth_data[0]));
    
    /*
      Insert the data into the new positions.
    */
    graph->node_id.insert(
      insert_index,
      move_nodes.data(),
      nodes_to_move
    );
    
    graph->parent_depth_data.insert(
      insert_index,
      move_parent_depth_data.data(),
      nodes_to_move
    );
    
    graph->local_transform.insert(
      insert_index,
      move_local_transform.data(),
      nodes_to_move
    );
    
    graph->world_transform.insert(
      insert_index,
      move_world_transform.data(),
      nodes_to_move
    );
    
    graph->data.insert(
      insert_index,
      data_to_move.data(),
      nodes_to_move
    );
    
    #ifndef NDEBUG
    graph_size_check(graph);
    #endif
    
    /*
      Calculate the depth differences between old and new parents.
      If parent_id = 0 aka root node, we need to add an extra -1 to get the
      correct depth.
    */
    const int32_t old_parent_depth = this_depth - 1;
    const int32_t parent_diff      = parent_depth - old_parent_depth;
    const int32_t depth_diff       = parent_diff - (parent_id ? 0 : 1);
    
    // Update the depth data
    for(uint32_t i = 0; i < nodes_to_move; ++i)
    {
      const uint64_t old_data  = move_parent_depth_data[i];
      const uint32_t old_depth = get_depth(old_data);
      const uint32_t depth     = old_depth + depth_diff;
      const uint64_t new_data  = set_data(get_parent_id(old_data), depth);
      const size_t   index     = insert_index + i;
      
      graph->parent_depth_data[index] = new_data;
    }
    
    const bool update_transform = node_recalc_transform_branch(
      graph,
      this_id
    );
    
    return update_transform;
  }
  
  return false;
}


size_t
node_child_count(
  const Data *graph,
  const uint32_t node_id)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);

  // -- Count Children -- //
  size_t child_count = 0;
  size_t index = 0;
  
  if(node_id > 0)
  {
    if(!lib::key::linear_search(
        node_id,
        graph->node_id.data(),
        graph->node_id.size(),
        &index))
    {
      LOG_ERROR("Failed to find node");
      return 0;
    }
  }
  
  // Calculate children.
  {
    const int64_t this_depth = node_id ? (int64_t)get_depth(graph->parent_depth_data[index]) : -1;
    const size_t start_index = node_id ? index + 1 : 0;
    
    const size_t count = graph->node_id.size();
    
    for(size_t i = start_index; i < count; ++i)
    {
      const int64_t that_depth = get_depth(graph->parent_depth_data[i]);
    
      if(that_depth > this_depth)
      {
        if(that_depth == this_depth + 1)
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


size_t
node_descendants_count(
  const Data *graph,
  const uint32_t node_id)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);

  // -- Count all decendants -- //
  size_t child_count = 0;
  size_t index = 0;
  
  if(node_id > 0)
  {
  if(!lib::key::linear_search(
      node_id,
      graph->node_id.data(),
      graph->node_id.size(),
      &index))
    {
      LOG_ERROR("Failed to find node");
      return 0;
    }
  }
  
  // Calculate descendants  
  {
    const int32_t this_depth = node_id ? get_depth(graph->parent_depth_data[index]) : -1;
    const size_t start = this_depth >= 0 ? index + 1 : 0;
    
    for(size_t i = start; i < graph->node_id.size(); ++i)
    {
      const int32_t that_depth = get_depth(graph->parent_depth_data[i]);
    
      if(that_depth > this_depth)
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
node_get_child(
  const Data *graph,
  const uint32_t node_id,
  const size_t child_index)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);

  // -- Count Children -- //
  size_t child_count = 0;
  size_t index = 0;
  
  if(node_id > 0)
  {
    if(!lib::key::linear_search(
        node_id,
        graph->node_id.data(),
        graph->node_id.size(),
        &index))
    {
      LOG_ERROR("Failed to find node");
      return 0;
    }
  }
  
  // Calculate children.
  {
    const int64_t this_depth = node_id ? (uint64_t)get_depth(graph->parent_depth_data[index]) : -1;
    const size_t start_index = node_id ? index + 1 : 0;
    
    for(size_t i = start_index; i < graph->node_id.size(); ++i)
    {
      const int64_t that_depth = get_depth(graph->parent_depth_data[i]);
    
      if(that_depth > this_depth)
      {
        if(that_depth == this_depth + 1)
        {
          if(child_count == child_index)
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
node_get_parent(const Data *graph, const uint32_t node_id)
{
  size_t index = 0;

  if(node_exists(graph, node_id, &index))
  {
    return get_parent_id(graph->parent_depth_data[index]);
  }

  return 0;
}


// ------------------------------------------------------------ [ Callbacks ] --


bool
callback_graph_tick(Data *data, const graph_tick_fn &cb, uintptr_t user_data)
{
  data->frame_tick_callbacks.emplace_back(
    cb,
    user_data
  );
  
  return true;
}


bool
callback_node_delete(Data *data, const node_delete_fn &cb, uintptr_t user_data)
{
  data->node_delete_callbacks.emplace_back(
    cb,
    user_data
  );
  
  return true;
}


// ----------------------------------------------------------- [ Attributes ] --


bool
node_get_name(
  const Data *data,
  const uint32_t node_id,
  const char **name)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    *name = data->data[index].name.data;
    return true;
  }
  
  return false;
}

  
bool
node_set_name(
  Data *data,
  const uint32_t node_id,
  const char *name)
{
  size_t index = 0;
  
  const size_t str_len = strlen(name) + 1;
  char clipped[16]{0};
  strlcpy(clipped, name, str_len > 16 ? 16 : str_len);
  
  if(node_exists(data, node_id, &index))
  {
    strlcpy(data->data[index].name.data, clipped, str_len);
    return true;
  }
  
  return false;
}


bool
node_get_transform(
  const Data *data,
  const uint32_t node_id,
  math::transform *trans,
  const bool inherited)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    if(!inherited)
    {
      *trans = data->local_transform[index];
    }
    else
    {
      *trans = data->world_transform[index];
    }
    return true;
  }
  
  return false;
}


bool
node_set_transform(
  Data *graph,
  const uint32_t node_id,
  const math::transform *trans)
{
  size_t index = 0;
  const bool found = node_exists(graph, node_id, &index);
  
  if(found)
  {
    graph->local_transform[index] = *trans;

    return node_recalc_transform_branch(
      graph,
      node_id
    );
  }
  
  return false;
}


bool
node_register_type(
  Data *data,
  const uint32_t node_id,
  const uint64_t type_id)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    data->data[index].node_type_id |= type_id;
    
    return true;
  }
  
  return false;
}


bool
node_unregister_type(
  Data *data,
  const uint32_t node_id,
  const uint64_t type_id)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    data->data[index].node_type_id &= ~type_id;
    
    return true;
  }
  
  return false;
}


bool
node_get_data_type_id(
  const Data *data,
  const uint32_t node_id,
  uint64_t *type_id)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    *type_id = data->data[index].node_type_id;
    return true;
  }
  
  return false;
}


bool
node_get_user_data(
  const Data *data,
  const uint32_t node_id,
  uintptr_t *user_data)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    *user_data = data->data[index].user_data;
    return true;
  }

  return false;
}


bool
node_set_user_data(
  Data *data,
  const uint32_t node_id,
  const uintptr_t *user_data)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    data->data[index].user_data = *user_data;
    return true;
  }
  
  return false;
}


} // ns
} // ns
