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


} // ns
} // ns


#endif // inc guard
