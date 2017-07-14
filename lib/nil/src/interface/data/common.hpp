#ifndef COMMON_INCLUDED_EA2E7D59_6069_434A_B35C_2B65A84B48AA
#define COMMON_INCLUDED_EA2E7D59_6069_434A_B35C_2B65A84B48AA


#include <nil/node.hpp>
#include <nil/data/transform.hpp>
#include <graph/graph_data.hpp>
#include <data/internal_data.hpp>
#include <lib/key.hpp>
#include <lib/optimizations.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>
#include <stddef.h>


#define NIL_DATA_GETTER_SETTER_HAS_SETUP  \
  Graph::Data *graph = get_graph_data();  \
  LIB_ASSERT(graph);                      \
  LIB_ASSERT(node.is_valid());

#define NIL_DATA_GETTER_ERROR(NAME) \
  LOG_ERROR("Node does not contain " #NAME " data");

#define NIL_DATA_SETTER_ERROR(NAME) \
  LOG_ERROR("Failed to set " #NAME " data to node");


namespace Nil {
namespace Data {


using setter_cb = void(*)(uint32_t id, uintptr_t user_data, size_t index);


inline bool
find_node(
  const Nil::Node &node,
  uint32_t *keys,
  size_t key_size,
  size_t *index = nullptr)
{
  const uint32_t id   = node.get_id();
  
  const bool found = lib::key::linear_search(
    id,
    keys,
    key_size,
    index
  );
  
  return found;
}


template<typename T, typename S, typename U>
inline void
get_data_helper(
  const Node &node,
  T &out,
  uint32_t *keys,
  size_t key_size,
  uintptr_t user_data,
  const S &get,
  const U &not_found)
{
  size_t out_index = 0;

  const bool found = find_node(
    node,
    keys,
    key_size,
    &out_index
  );

  if(likely(found))
  {
    // Update existing data
    get(out_index, out, user_data);
  }
  else
  {
    // Insert new data.
    not_found(node.get_id(), out, user_data);
  }
}



template<typename T, typename S, typename U>
inline void
set_data_helper(
  const Node &node,
  T &in,
  uint32_t *keys,
  size_t key_size,
  uint64_t type_id,
  uintptr_t user_data,
  const S &update,
  const U &insert)
{
  size_t out_index = 0;
  
  const bool found = find_node(
    node,
    keys,
    key_size,
    &out_index
  );

  if(found)
  {
    // Update existing data.
    update(out_index, in, user_data);
  }
  else
  {
    Graph::node_register_type(
        Nil::Data::get_graph_data(),
        node.get_id(),
        type_id
      );
  
    // Insert new data.
    insert(node.get_id(), in, user_data);
  }
  
  // Signal graph this is an updated node.
  Graph::data_updated(
    Nil::Data::get_graph_data(),
    node.get_id(),
    type_id
  );
}


template<typename S, typename U>
inline void
remove_data_helper(
  const Node &node,
  uint32_t *keys,
  size_t key_size,
  uint64_t type_id,
  uintptr_t user_data,
  const S &remove,
  const U &not_found)
{
  size_t out_index = 0;
  
  const bool found = find_node(
    node,
    keys,
    key_size,
    &out_index
  );

  if(found)
  {
    Graph::node_unregister_type(
      Nil::Data::get_graph_data(),
      node.get_id(),
      type_id
    );
  
    // Remove data.
    remove(out_index, user_data);
  }
  else
  {
    // Not found.
    not_found(node.get_id(), user_data);
  }
}


/*!
  Generic_data
  --
  Yes yes templates bad!
  The hope is I could do a generic data managment then things that require
  specific needs can be custom.
  
  TODO
  --
  - Sort by id so can bin search
  - process_event_lists() should happen via callback after api has had its turn.
  - Mask events, such as ADDED | UPDATED some people just want to read not write.
    - eg camera on window changes.
    - Need better memory mgr for this stuff.
*/
template<typename T>
struct Generic_data
{
  // -- Data -- //

  lib::array<uint32_t>  keys;
  lib::array<T>         data;
  lib::array<uint32_t>  actions;
  
  uint64_t              type_id;
  
  // -- Cached Event Data -- //
  
  std::vector<Nil::Node>  added_nodes;
  lib::array<T>           added_data;
  
  std::vector<Nil::Node>  updated_nodes;
  lib::array<T>           updated_data;
  
  std::vector<Nil::Node>  removed_nodes;
  lib::array<T>           removed_data;
  
  // --
  
  setter_cb setter_fn = nullptr;
  
  explicit
  Generic_data(
    const uint64_t dependent_types = 0,
    const setter_cb &set_cb = nullptr
  )
  {
    setter_fn = set_cb;
  
    type_id = Nil::Graph::data_register_type(
      Nil::Data::get_graph_data(),
      
      // Tick Callback
      [](uintptr_t user_data)
      {
        Generic_data<T> *data = reinterpret_cast<Generic_data<T>*>(user_data);
        LIB_ASSERT(data);
        
        // -- Clear Actions -- //
        
        memset(
          data->actions.data(),
          0,
          sizeof(decltype(data->actions[0])) * data->actions.size()
        );
        
        // -- Clear Event Caches -- //
        
        data->added_nodes.clear();
        data->added_data.clear();
        data->updated_data.clear();
        data->updated_nodes.clear();
        data->removed_nodes.clear();
        data->removed_data.clear();
      },
      
      // Node deleted callback
      [](const uint32_t id, uintptr_t user_data)
      {
        Generic_data<T> *data = reinterpret_cast<Generic_data<T>*>(user_data);
        LIB_ASSERT(data);
        
        data->remove_data(Nil::Node(id));
      },
      
      // Dependency callback
      [](const uint32_t id, uintptr_t user_data)
      {
        Generic_data<T> *data = reinterpret_cast<Generic_data<T>*>(user_data);
        LIB_ASSERT(data);
      
        size_t index = 0;
        const uint32_t *ids = data->keys.data();
        const size_t id_count = data->keys.size();
        
        const bool found = lib::key::linear_search(
          id,
          ids,
          id_count,
          &index
        );
        
        if(found)
        {
          data->actions[index] |= Nil::Data::Event::UPDATED;
          
          if(data->setter_fn)
          {
            data->setter_fn(id, user_data, index);
          }
        }
      },
      
      (uintptr_t)this,
      dependent_types
    );
  }
  
  
  ~Generic_data()
  {
    setter_fn = nullptr;
    
    Nil::Graph::data_unregister_type(
      Nil::Data::get_graph_data(),
      type_id
    );
  }
  
  
  void
  process_event_lists()
  {
    const size_t count = keys.size();
  
    for(size_t i = 0; i < count; ++i)
    {
      const uint32_t action = actions[i];
    
      if(action & Nil::Data::Event::ADDED)
      {
        added_nodes.emplace_back(keys[i]);
        added_data.emplace_back(data[i]);
      }
        
      else if(action & Nil::Data::Event::UPDATED)
      {
        updated_nodes.emplace_back(keys[i]);
        updated_data.emplace_back(data[i]);
      }
      
      else if(action & Nil::Data::Event::REMOVED)
      {
        removed_nodes.emplace_back(keys[i]);
        removed_data.emplace_back(data[i]);
      }
    }
  }
  
  
  void
  get_access(size_t *count, T **access)
  {
    *count = data.size();
    *access = data.begin();
  }
  
  
  void
  get_data(const Nil::Node &node, T &out)
  {
    get_data_helper(
      node,
      out,
      keys.data(),
      keys.size(),
      (uintptr_t)this,
      
      // Get
      [](const size_t index, T &out, uintptr_t user_data)
      {
        Generic_data<T> *self = reinterpret_cast<Generic_data<T>*>(user_data);
        
        out = self->data[index];
      },
      
      [](const uint32_t node_id, T &out, uintptr_t user_data)
      {
        LOG_ERROR("Failed to find");
      }
    );
  }
  
  
  void
  set_data(Nil::Node &node, const T &in)
  {
    set_data_helper(
      node,
      in,
      
      keys.data(),
      keys.size(),
      type_id,
      (uintptr_t)this,
      
      // Update Current
      [](const size_t index, const T &in, const uintptr_t user_data)
      {
        Generic_data<T> *self = reinterpret_cast<Generic_data<T>*>(user_data);
      
        self->data[index] = in;
        self->actions[index] |= Nil::Data::Event::UPDATED;
      },
      
      // Insert New
      [](const uint32_t node_id, const T &in, const uintptr_t user_data)
      {
        Generic_data<T> *self = reinterpret_cast<Generic_data<T>*>(user_data);
        
        self->keys.emplace_back(node_id);
        self->data.emplace_back(in);
        self->actions.emplace_back(Nil::Data::Event::ADDED);
      }
    );
  }
  
  
  void
  remove_data(const Nil::Node &node)
  {
    remove_data_helper(
      node,
      keys.data(),
      keys.size(),
      type_id,
      (uintptr_t)this,
      
      // Remove
      [](const size_t index, uintptr_t user_data)
      {
        Generic_data<T> *self = reinterpret_cast<Generic_data<T>*>(user_data);
      
        self->removed_nodes.emplace_back(
          Nil::Node(self->keys[index])
        );
        
        self->removed_data.emplace_back(
          self->data[index]
        );

        self->data.erase(index);
        self->keys.erase(index);
      },
      
      // Not found
      [](const uint32_t node_id, uintptr_t user_data)
      {
        LOG_ERROR("Node not found")
      }
    );
  }
  
  
  void
  events(const uint32_t event, size_t *count, T **out_data, Nil::Node **out_node)
  {
    process_event_lists();
  
    switch(event)
    {
    case(Nil::Data::Event::ADDED):
      *count = added_data.size();
      if(out_data) { *out_data = added_data.data();  }
      if(out_node) { *out_node = added_nodes.data(); }
      break;

    case(Nil::Data::Event::UPDATED):
      *count = updated_data.size();
      if(out_data) { *out_data = updated_data.data();  }
      if(out_node) { *out_node = updated_nodes.data(); }
      break;
      
    case(Nil::Data::Event::REMOVED):
      *count = removed_data.size();
      if(out_data) { *out_data = removed_data.data();  }
      if(out_node) { *out_node = removed_nodes.data(); }
      break;
    }
  }
  
};


} // ns
} // ns


#endif // inc guard
