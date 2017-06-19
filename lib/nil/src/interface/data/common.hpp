#ifndef COMMON_INCLUDED_EA2E7D59_6069_434A_B35C_2B65A84B48AA
#define COMMON_INCLUDED_EA2E7D59_6069_434A_B35C_2B65A84B48AA


#include <nil/node.hpp>
#include <graph/graph_data.hpp>
#include <data/data.hpp>
#include <stddef.h>


#define NIL_DATA_GETTER_SETTER_HAS_SETUP  \
  Graph::Data *graph = get_graph_data();  \
  LIB_ASSERT(graph);                      \
  LIB_ASSERT(node.is_valid());
  
#define NIL_DATA_TYPE_ID_REG                                \
  const static uint64_t type_id = Data::register_type_id(); \
  LIB_ASSERT(type_id);                                      \
  return type_id;

#define NIL_DATA_GETTER_ERROR(NAME) \
  LOG_ERROR("Node does not contain " #NAME " data");

#define NIL_DATA_SETTER_ERROR(NAME) \
  LOG_ERROR("Failed to set " #NAME " data to node");


namespace Nil {
namespace Data {



template<typename S, typename T, typename U>
bool
getter_helper(const uint32_t id, const S &id_arr, const T &data_arr, U &out)
{
  size_t index = 0;
  
  if(lib::key::linear_search(id, id_arr.data(), id_arr.size(), &index))
  {
    out = data_arr[index];
    return true;
  }

  return false;
}


template<typename S, typename T, typename U>
bool
setter_helper(Node node, S &id_arr, T &data_arr, const U &in, const uint64_t data_type_id)
{
  size_t index = 0;
  
  if(lib::key::linear_search(node.get_id(), id_arr.data(), id_arr.size(), &index))
  {
    data_arr[index] = in;
  }
  else
  {
    id_arr.emplace_back(node.get_id());
    data_arr.emplace_back(in);
    
    const uint64_t id = node.get_data_type_id();
    node.set_data_type_id(id | data_type_id);
  }
  
  LIB_ASSERT(id_arr.size() == data_arr.size());
  
  /*
    Add Event
  */
  Graph::Data *graph = Data::get_graph_data();
  LIB_ASSERT(graph);
  
  Graph::node_modified(graph, node.get_id());
  
  return true;
}


template<typename S>
bool
has(const uint32_t id, const S &id_arr)
{
  if(lib::key::linear_search(
    id,
    id_arr.data(),
    id_arr.size()))
  {
    return true;
  }
  
  return false;
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
  
  explicit
  Generic_data(const uint64_t dependent_types = 0)
  {
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
        }
      },
      
      (uintptr_t)this,
      dependent_types
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
  get_data(const Nil::Node &node, T &out)
  {
    size_t out_index = 0;

    const bool found = find(node, &out_index);

    if(likely(found))
    {
      out = data[out_index];
    }
    else
    {
      NIL_DATA_GETTER_ERROR(Window)
    }
  }
  
  
  void
  set_data(Nil::Node &node, const T &in)
  {
    size_t out_index = 0;

    const bool found = find(node, &out_index);

    if(found)
    {
      data[out_index] = in;
      actions[out_index] |= Nil::Data::Event::UPDATED;
    }
    else
    {
      uint64_t type_ids = 0;
      
      Graph::node_get_data_type_id(
        Nil::Data::get_graph_data(),
        node.get_id(),
        &type_ids
      );
      
      type_ids |= get_type_id(in);
      
      Graph::node_set_data_type_id(
        Nil::Data::get_graph_data(),
        node.get_id(),
        &type_ids
      );
      
      keys.emplace_back(node.get_id());
      data.emplace_back(in);
      actions.emplace_back(Nil::Data::Event::ADDED);
    }
    
    Graph::data_updated(Nil::Data::get_graph_data(), node.get_id(), type_id);
  }
  
  
  void
  remove_data(const Nil::Node &node)
  {
    size_t out_index = 0;

    const bool found = find(node, &out_index);

    if(found)
    {
      uint64_t type_ids = 0;
      
      Graph::node_get_data_type_id(
        Nil::Data::get_graph_data(),
        node.get_id(),
        &type_ids
      );
      
      T dummy_data;
      
      type_ids &= ~get_type_id(dummy_data);
      
      Graph::node_set_data_type_id(
        Nil::Data::get_graph_data(),
        node.get_id(),
        &type_ids
      );
      
      actions[out_index] |= Nil::Data::Event::REMOVED;
    }
  }
  
  
  bool
  find(const Nil::Node &node, size_t *index = nullptr)
  {
    const uint32_t id   = node.get_id();
    const uint32_t *ids = keys.data();
    const size_t count  = keys.size();
    
    const bool found = lib::key::linear_search(
      id,
      ids,
      count,
      index
    );
    
    return found;
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
