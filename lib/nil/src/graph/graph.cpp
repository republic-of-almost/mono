#include "graph.hpp"
#include "graph_data.hpp"
#include <lib/utilities.hpp>
#include <nil/data/window.hpp>
#include <nil/data/transform.hpp>
#include <nil/data/bounding_box.hpp>


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
    LIB_ASSERT(graph->node_id.size() == graph->name.size());
    LIB_ASSERT(graph->node_id.size() == graph->local_transform.size());
    LIB_ASSERT(graph->node_id.size() == graph->world_transform.size());
    LIB_ASSERT(graph->node_id.size() == graph->bounding_box.size());
    LIB_ASSERT(graph->node_id.size() == graph->node_type_id.size());
    LIB_ASSERT(graph->node_id.size() == graph->user_data.size());
    LIB_ASSERT(graph->node_id.size() == graph->last_update.size());
    
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


//  inline uint64_t
//  set_parent_id(const uint64_t data, const uint32_t parent_id)
//  {
//    return lib::bits::pack3232(
//      get_depth(data),
//      parent_id   
//    );
//  }
//
//
//  inline uint64_t
//  set_depth(const uint64_t data, const uint32_t depth)
//  {
//    return lib::bits::pack3232(
//      depth,
//      get_parent_id(data)
//    );
//  }
  
  
  inline Graph::Event*
  add_event(Graph::Data *graph, uint32_t evt_id, uint32_t node_id)
  {
    // Search for existing event.
    {
      const size_t count = graph->node_events.size();
      Graph::Event *events = graph->node_events.data();
    
      for(size_t i = 0; i < count; ++i)
      {
        Graph::Event *evt = &events[i];
      
        if(evt->node_id == node_id)
        {
          const uint32_t curr_events = evt->event_action;
        
          if(!(curr_events & evt_id))
          {
            evt->event_action = curr_events | evt_id;
          }
          
          return evt;
        }
      }
    }
    
    graph->node_events.emplace_back(
      evt_id,
      node_id
    );
    memset(graph->node_events.back().name.data, 0, sizeof(short_string));
    graph->node_events.back().last_update = graph->graph_tick;
    
    graph->node_events.back().transform = math::transform_init();
    
    return &graph->node_events.back();
  }

} // anon ns


// ------------------------------------------------------------ [ Life time ] --


void
initialize(Data *graph)
{
  graph->instance_counter = 0;
  graph->graph_tick = 0;
}


namespace
{
  void
  graph_add(Graph::Data *graph, const Graph::Event *evt)
  {
    static short_string default_name {"Node"};
  
    graph->node_id.emplace_back(evt->node_id);
    graph->parent_depth_data.emplace_back(uint64_t{0});
    graph->name.emplace_back(strlen(evt->name.data) > 0 ? evt->name : default_name);
    graph->local_transform.emplace_back(evt->transform);
    graph->world_transform.emplace_back(evt->transform);
    graph->bounding_box.emplace_back(evt->boundinb_box);
    graph->node_type_id.emplace_back(evt->node_type_id);
    graph->user_data.emplace_back(evt->user_data);
    graph->last_update.emplace_back(evt->last_update);

    #ifndef NDEBUG
    graph_size_check(graph);
    #endif
  }
  
  
  void
  graph_updated(Graph::Data *graph, const Graph::Event *evt)
  {
    // Update transforms //
    // Find this node positions
    // Find number of chilren
    
    // -- Properties -- //
    const uint32_t this_id = evt->node_id;
    uint32_t parent_id = 0;

    size_t this_index = 0;
    uint32_t this_depth = 0;
    {
      if(!lib::key::linear_search(
        this_id,
        graph->node_id.data(),
        graph->node_id.size(),
        &this_index))
      {
        return;
      }
      
      this_depth = get_depth(graph->parent_depth_data[this_index]);
      parent_id = get_parent_id(graph->parent_depth_data[this_index]);
    }
    
    uint32_t nodes_to_update = node_descendants_count(graph, this_id) + 1;
  
    // -- //
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
          return;
        }

        insert_index = parent_index + 1;
        parent_depth = get_depth(graph->parent_depth_data[parent_index]);
      }
    }
    
    // Update Transforms //
    {
      lib::array<math::transform, stack_hint> transform_stack;
      
      if(parent_index)
      {
        transform_stack.emplace_back(graph->world_transform[parent_index]);
      }
      else
      {
        transform_stack.emplace_back(math::transform_init());
      }
      
      uint32_t curr_depth = this_depth;
      
      for(uint32_t i = 0; i < nodes_to_update; ++i)
      {
        const size_t   index = this_index + i;
        const uint64_t data  = graph->parent_depth_data[index];
        const uint32_t depth = get_depth(data);
        
        // Pop off all unrequired transforms.
        while(curr_depth > depth)
        {
          transform_stack.pop_back();
          curr_depth -= 1;
        }
        
        // Calc new world transform.
        const math::transform child_world(
          math::transform_inherited(
            transform_stack.top(),
            graph->local_transform[index]
          )
        );
        
        graph->world_transform[index] = child_world;
        
        transform_stack.emplace_back(child_world);
        curr_depth += 1;
      }
    }
  }
  
  
  void
  graph_move(Graph::Data *graph, const Graph::Event *evt)
  {
    // -- Properties -- //
    const uint32_t this_id = evt->node_id;
    const uint32_t parent_id = evt->parent;
  
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
        return;
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
      
      lib::array<short_string, stack_hint> move_name(
        graph->name.begin() + this_index,
        graph->name.begin() + (this_index + nodes_to_move)
      );
      graph->name.erase(this_index, nodes_to_move);
      
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
      
      lib::array<math::aabb, stack_hint> move_bounding_box(
        graph->bounding_box.begin() + this_index,
        graph->bounding_box.begin() + (this_index + nodes_to_move)
      );
      graph->bounding_box.erase(this_index, nodes_to_move);
      
      lib::array<uint64_t, stack_hint> move_node_type_id(
        graph->node_type_id.begin() + this_index,
        graph->node_type_id.begin() + (this_index + nodes_to_move)
      );
      graph->node_type_id.erase(this_index, nodes_to_move);
      
      lib::array<uintptr_t, stack_hint> move_user_data(
        graph->user_data.begin() + this_index,
        graph->user_data.begin() + (this_index + nodes_to_move)
      );
      graph->user_data.erase(this_index, nodes_to_move);
      
      lib::array<uint64_t, stack_hint> move_last_update(
        graph->last_update.begin() + this_index,
        graph->last_update.begin() + (this_index + nodes_to_move)
      );
      graph->last_update.erase(this_index, nodes_to_move);
      
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
            return;
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

      graph->name.insert(
        insert_index,
        move_name.data(),
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
      
      graph->bounding_box.insert(
        insert_index,
        move_bounding_box.data(),
        nodes_to_move
      );
      
      graph->node_type_id.insert(
        insert_index,
        move_node_type_id.data(),
        nodes_to_move
      );
      
      graph->user_data.insert(
        insert_index,
        move_user_data.data(),
        nodes_to_move
      );
      
      graph->last_update.insert(
        insert_index,
        move_last_update.data(),
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
      
      // Update Last Modified
      for(uint32_t i = 0; i < nodes_to_move; ++i)
      {
        graph->last_update[i] = graph->graph_tick;
      }
      
      // -- Update transforms -- //
      /*
        Transforms are nested so we keep a stack of transforms that represent
        the parent transform, we pop off when we walk up the tree.
      */
      {
        lib::array<math::transform, stack_hint> transform_stack;
        transform_stack.emplace_back(graph->world_transform[parent_index]);
        
        uint32_t curr_depth = parent_depth;
        
        for(uint32_t i = 0; i < nodes_to_move; ++i)
        {
          const size_t index  = insert_index + i;
          const uint64_t data = graph->parent_depth_data[index];
          const uint32_t depth = get_depth(data);
          
          // Pop off all unrequired transforms.
          while((curr_depth + 1) < depth)
          {
            transform_stack.pop_back();
            curr_depth -= 1;
          }
          
          curr_depth = depth;
        
          // Calc new world transform.
          const math::transform child_world(
            math::transform_inherited(
              transform_stack.top(),
              graph->local_transform[index]
            )
          );
          
          graph->world_transform[index] = child_world;
          
          transform_stack.emplace_back(child_world);
        }
      }
    }
  }
  
  
  void
  graph_remove(Graph::Data *graph, const Graph::Event *evt)
  {
    const uint32_t node_id = evt->node_id;
  
    // -- Remove The Node -- //
    size_t index = 0;

    if(node_exists(graph, node_id, &index))
    {
      graph->node_id.erase(index);
      graph->name.erase(index);
      graph->parent_depth_data.erase(index);
      graph->local_transform.erase(index);
      graph->world_transform.erase(index);
      graph->bounding_box.erase(index);
      graph->node_type_id.erase(index);
      graph->user_data.erase(index);
      graph->last_update.erase(index);

      #ifndef NDEBUG
      graph_size_check(graph);
      #endif
    }
    
    // -- Remove The Data -- //
    {
      Component_data &data = graph->component_data;
      
      // Audio Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.audio_node_id.data(),
          data.audio_node_id.size(), &index))
        {
          data.audio_node_id.erase(index);
          data.audio_data.erase(index);
        }
      }
      
      // Audio Resource Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.audio_resource_node_id.data(),
          data.audio_resource_node_id.size(), &index))
        {
          data.audio_resource_node_id.erase(index);
          data.audio_resource_data.erase(index);
        }
      }
      
      // Camera Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.camera_node_id.data(),
          data.camera_node_id.size(), &index))
        {
          data.camera_node_id.erase(index);
          data.camera_data.erase(index);
        }
      }
      
      // Collider Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.collider_node_id.data(),
          data.collider_node_id.size(), &index))
        {
          data.collider_node_id.erase(index);
          data.collider_data.erase(index);
        }
      }
      
      // Developer Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.developer_node_id.data(),
          data.developer_node_id.size(), &index))
        {
          data.developer_node_id.erase(index);
          data.developer_data.erase(index);
        }
      }
      
      // Gamepad Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.gamepad_node_id.data(),
          data.gamepad_node_id.size(), &index))
        {
          data.gamepad_node_id.erase(index);
          data.gamepad_data.erase(index);
        }
      }
      
      // Graphics Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.graphics_node_id.data(),
          data.graphics_node_id.size(), &index))
        {
          data.graphics_node_id.erase(index);
          data.graphics_data.erase(index);
        }
      }
      
      // Keyboard Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.keyboard_node_id.data(),
          data.keyboard_node_id.size(), &index))
        {
          data.keyboard_node_id.erase(index);
          data.keyboard_data.erase(index);
        }
      }
      
      // Light Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.light_node_id.data(),
          data.light_node_id.size(), &index))
        {
          data.light_node_id.erase(index);
          data.light_data.erase(index);
        }
      }
      
      // Logic Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.logic_node_id.data(),
          data.logic_node_id.size(), &index))
        {
          data.logic_node_id.erase(index);
          data.logic_data.erase(index);
        }
      }
      
      // Material Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.material_node_id.data(),
          data.material_node_id.size(), &index))
        {
          data.material_node_id.erase(index);
          data.material_data.erase(index);
        }
      }
      
      // Mesh Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.mesh_node_id.data(),
          data.mesh_node_id.size(), &index))
        {
          data.mesh_node_id.erase(index);
          data.mesh_data.erase(index);
        }
      }
      
      // Mesh Resource Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.mesh_resource_node_id.data(),
          data.mesh_resource_node_id.size(), &index))
        {
          data.mesh_resource_node_id.erase(index);
          data.mesh_data.erase(index);
        }
      }
      
      // Mouse Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.mouse_node_id.data(),
          data.mouse_node_id.size(), &index))
        {
          data.mouse_node_id.erase(index);
          data.mouse_data.erase(index);
        }
      }
      
      // Resource Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.resource_node_id.data(),
          data.resource_node_id.size(), &index))
        {
          data.resource_node_id.erase(index);
          data.resource_data.erase(index);
        }
      }
      
      // Rigidbody Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.rigidbody_node_id.data(),
          data.rigidbody_node_id.size(), &index))
        {
          data.rigidbody_node_id.erase(index);
          data.rigidbody_data.erase(index);
        }
      }
      
      // Texture Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.texture_node_id.data(),
          data.texture_node_id.size(), &index))
        {
          data.texture_node_id.erase(index);
          data.texture_data.erase(index);
        }
      }
      
      // Texture Resource Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.texture_resource_node_id.data(),
          data.texture_resource_node_id.size(), &index))
        {
          data.texture_resource_node_id.erase(index);
          data.texture_resource_data.erase(index);
        }
      }
      
      // Window Data
      {
        size_t index = 0;
        
        if(lib::key::linear_search(
          node_id,
          data.window_node_id.data(),
          data.window_node_id.size(), &index))
        {
          data.window_node_id.erase(index);
          data.window_data.erase(index);
        }
      }
    }
  }
  
} // ns


void
think(Data *graph)
{
  ++(graph->graph_tick);
  
  /*
    Graph events. we have todo it like this to make sure 
    that all nodes exist before move operations happen.
    otherwise we get into a situation where parenting tries to happen
    and the parent has yet to be created.
  */
  {
    for(const Graph::Event &event : graph->node_events)
    {
      if(event.event_action & Graph::Event::ADDED)
      {
        graph_add(graph, &event);
      }
      
      size_t index = 0;
      if(node_exists(graph, event.node_id, &index))
      {
        graph->last_update[index] = graph->graph_tick;
      }
    }
    
    for(const Graph::Event &event : graph->node_events)
    {
      if(event.event_action & Graph::Event::MOVED)
      {
        graph_move(graph, &event);
      }
      
      size_t index = 0;
      if(node_exists(graph, event.node_id, &index))
      {
        graph->last_update[index] = graph->graph_tick;
      }
    }

    for(const Graph::Event &event : graph->node_events)
    {
      if(event.event_action & Graph::Event::REMOVED)
      {
        graph_remove(graph, &event);
      }
      
      size_t index = 0;
      if(node_exists(graph, event.node_id, &index))
      {
        graph->last_update[index] = graph->graph_tick;
      }
    }
    
    for(const Graph::Event &event : graph->node_events)
    {
      if(event.event_action & Graph::Event::UPDATED_DATA)
      {
        graph_updated(graph, &event);
      }
      
      size_t index = 0;
      if(node_exists(graph, event.node_id, &index))
      {
        graph->last_update[index] = graph->graph_tick;
      }
    }
  }
  
  
  // Temp
  graph->node_events.clear();
}


void
destroy(Data *graph)
{
}


uint64_t
last_tick(Data *graph)
{
  return graph->graph_tick;
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


bool
node_pending(
  const Data *data,
  const uint32_t node_id,
  size_t *index)
{
  size_t count = 0;

  for(const auto &evt : data->node_events)
  {
    if(evt.node_id == node_id)
    {
      if(index)
      {
        *index = count;
      }
    
      return true;
    }
    
    ++count;
  }
  
  return false;
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
  
  // -- Create Event -- //
  add_event(graph, Graph::Event::ADDED, new_id);

  return new_id;
}


bool
node_remove(Data *graph, const uint32_t node_id)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(node_id);
  
  // -- Remove The Node -- //
  if(node_exists(graph, node_id) || node_pending(graph, node_id))
  {
    // -- Create Event -- //
    add_event(graph, Graph::Event::REMOVED, node_id);
  
    return true;
  }

  return false;
}


bool
node_set_parent(
  Data *graph,
  const uint32_t parent_id,
  const uint32_t this_id)
{
  // -- Param Check -- //
  LIB_ASSERT(graph);
  LIB_ASSERT(this_id);
  
  // -- Create Event -- //
  add_event(graph, Graph::Event::MOVED, this_id)->parent = parent_id;


  return true;
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


bool
node_modified(Data *data, const uint32_t node_id)
{
  add_event(data, Event::UPDATED_DATA, node_id);

//  Event evt{};
//  evt.node_id = node_id;
//  evt.event_action  |= Event::UPDATED_DATA;
//  data->node_events.emplace_back(evt);
  
  return true; // ?
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
    *name = data->name[index].data;
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    *name = data->node_events[index].name.data;
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
    strlcpy(data->name[index].data, clipped, str_len);
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    strlcpy(data->node_events[index].name.data, clipped, str_len);
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
    return true;
  }
  
  return false;
}


bool
node_get_transform(
  const Data *data,
  const uint32_t node_id,
  math::transform *trans,
  const bool world)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    if(!world)
    {
      *trans = data->local_transform[index];
    }
    else
    {
      *trans = data->world_transform[index];
    }
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    *trans = data->node_events[index].transform;
    return true;
  }
  
  return false;
}


bool
node_set_transform(
  Data *data,
  const uint32_t node_id,
  const math::transform *trans)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    const size_t child_count = node_descendants_count(data, node_id);
    
    data->local_transform[index] = *trans;
    
    for(uint32_t i = 0; i < child_count + 1; ++i)
    {
      const size_t update = index + i;
      add_event(data, Graph::Event::UPDATED_DATA, data->node_id[update]);
    }
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    data->node_events[index].transform = *trans;
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
    return true;
  }
  
  return false;
}


bool
node_get_bounding_box(
  const Data *data,
  const uint32_t node_id,
  math::aabb *aabb)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    *aabb = data->bounding_box[index];
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    *aabb = data->node_events[index].boundinb_box;
    return true;
  }
  
  return false;
}


bool
node_set_bounding_box(
  Data *data,
  const uint32_t node_id,
  const math::aabb *aabb)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    data->bounding_box[index] = *aabb;
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    data->node_events[index].boundinb_box = *aabb;
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
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
    *type_id = data->node_type_id[index];
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    *type_id = data->node_events[index].node_type_id;
    return true;
  }
  
  return false;
}


bool
node_set_data_type_id(
  Data *data,
  const uint32_t node_id,
  const uint64_t *type_id)
{
  size_t index = 0;
  
  if(node_exists(data, node_id, &index))
  {
    data->node_type_id[index] = *type_id;
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    data->node_events[index].node_type_id = *type_id;
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
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
    *user_data = data->user_data[index];
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    *user_data = data->node_events[index].user_data;
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
    data->user_data[index] = *user_data;
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
    return true;
  }
  else if(node_pending(data, node_id, &index))
  {
    data->user_data[index] = *user_data;
    add_event(data, Graph::Event::UPDATED_DATA, node_id);
    return true;
  }
  
  return false;
}


} // ns
} // ns
