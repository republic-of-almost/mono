#include <nil/data/bounding_box.hpp>
#include <nil/node.hpp>
#include <data/internal_data.hpp>
#include <graph/graph.hpp>
#include <math/geometry/geometry.hpp>
#include <string.h>
#include "common.hpp"


// ----------------------------------------------------------------- [ Data ] --


namespace {


struct Bounding_box_data
{
  lib::array<uint32_t> keys;
  lib::array<uint32_t> actions;
  
  lib::array<Nil::Data::Bounding_box> local_bb;
  lib::array<Nil::Data::Bounding_box> world_bb;
  
  uint64_t type_id;
  
  // -- Cached Event Data -- //
  
  std::vector<Nil::Node>              added_bb_nodes;
  lib::array<Nil::Data::Bounding_box> added_bb_data;

  std::vector<Nil::Node>              updated_bb_nodes;
  lib::array<Nil::Data::Bounding_box> updated_bb_data;
  
  std::vector<Nil::Node>              removed_bb_nodes;
  lib::array<Nil::Data::Bounding_box> removed_bb_data;
  
  ~Bounding_box_data()
  {
    
  }
  
  explicit
  Bounding_box_data()
  {
    type_id = Nil::Graph::data_register_type(
      Nil::Data::get_graph_data(),
      
      // Tick Callback
      [](uintptr_t user_data)
      {
        Bounding_box_data *data = reinterpret_cast<Bounding_box_data*>(user_data);
        LIB_ASSERT(data);
        
        // -- Clear Actions -- //
        
//        memset(
//          data->actions.data(),
//          0,
//          sizeof(decltype(data->actions[0])) * data->actions.size()
//        );
//        
//        // -- Clear Event Caches -- //
//        
//        data->added_bb_nodes.clear();
//        data->added_bb_data.clear();
//        data->updated_bb_data.clear();
//        data->updated_bb_nodes.clear();
//        data->removed_bb_nodes.clear();
//        data->removed_bb_data.clear();
      },
      
      // Node deleted callback
      [](const uint32_t id, uintptr_t user_data)
      {
        Bounding_box_data *data = reinterpret_cast<Bounding_box_data*>(user_data);
        LIB_ASSERT(data);
        
        Nil::Data::remove_data_helper(
          Nil::Node(id),
          data->keys.data(),
          data->keys.size(),
          data->type_id,
          (uintptr_t)data,
          
          // Remove
          [](const size_t index, uintptr_t user_data)
          {
            Bounding_box_data *data = reinterpret_cast<Bounding_box_data*>(user_data);
            LIB_ASSERT(data);
            
            const size_t key_size = data->keys.size();
            LIB_ASSERT(index < key_size);
            
            data->keys.erase(index);
            data->local_bb.erase(index);
            data->world_bb.erase(index);
//            data->actions.erase(index);
          },
          
          // Not found
          [](const uint32_t node_id, uintptr_t user_data)
          {
            LOG_ERROR("Failed to remove Bounding box.")
          }
        );
      },
      
      // Dependency callback
      [](const uint32_t id, uintptr_t user_data)
      {
        Bounding_box_data *data = reinterpret_cast<Bounding_box_data*>(user_data);
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
//          data->actions[index] |= Nil::Data::Event::UPDATED;
          
          Nil::Node node(id);
          
          Nil::Data::Bounding_box in = data->local_bb[index];
          
          Nil::Data::Transform trans;
          Nil::Data::get(node, trans);
          
          const math::vec3 corners[]
          {
            math::vec3_init(in.min),
            math::vec3_init(in.max[0], in.min[1], in.min[2]),
            math::vec3_init(in.min[0], in.min[1], in.max[2]),
            math::vec3_init(in.max[0], in.min[1], in.max[2]),
            
            math::vec3_init(in.max),
            math::vec3_init(in.min[0], in.max[1], in.max[2]),
            math::vec3_init(in.max[0], in.max[1], in.min[2]),
            math::vec3_init(in.min[0], in.max[1], in.min[2]),
          };
          
          constexpr size_t count = sizeof(corners) / sizeof(decltype(corners[0]));
          
          const math::quat rot = math::quat_init(trans.rotation);
          
          math::vec3 rot_corners[count];
          
          for(int i = 0; i < count; ++i)
          {
            rot_corners[i] = math::quat_rotate_point(rot, corners[i]);
          }
          
          // -- Find new min max -- //
          
          float min[3] {math::float_max(),math::float_max(),math::float_max()};
          float max[3] {math::float_min(),math::float_min(),math::float_min()};
          
//          for(int i = 0; i < count; ++i)
          for(auto &rot_pt : rot_corners)
          {
            max[0] = math::max(max[0], math::get_x(rot_pt));
            max[1] = math::max(max[1], math::get_y(rot_pt));
            max[2] = math::max(max[2], math::get_z(rot_pt));
            
            min[0] = math::min(min[0], math::get_x(rot_pt));
            min[1] = math::min(min[1], math::get_y(rot_pt));
            min[2] = math::min(min[2], math::get_z(rot_pt));
          }
          
          // -- Scale -- //
          
          min[0] *= trans.scale[0];
          min[1] *= trans.scale[1];
          min[2] *= trans.scale[2];
          
          max[0] *= trans.scale[0];
          max[1] *= trans.scale[1];
          max[2] *= trans.scale[2];
          
          // -- Offset -- //
          
          min[0] += trans.position[0];
          min[1] += trans.position[1];
          min[2] += trans.position[2];
          
          max[0] += trans.position[0];
          max[1] += trans.position[1];
          max[2] += trans.position[2];
          
          // -- Save -- //
          
          Nil::Data::Bounding_box world_in = in;
          memcpy(world_in.min, min, sizeof(world_in.min));
          memcpy(world_in.max, max, sizeof(world_in.max));
        
          data->world_bb[index] = world_in;
        }
      },
      
      (uintptr_t)this,
      Nil::Data::get_type_id(Nil::Data::Transform{})
    );
  }
  
};


// -- Lazy Getter -- //
Bounding_box_data&
get_bb_data()
{
  static Bounding_box_data bb;
  return bb;
}


} // anon ns


namespace Nil {
namespace Data {


// -------------------------------------------------------------- [ Get/Set ] --


void
get(size_t *count, uint32_t **node_ids)
{
  *count = get_bb_data().keys.size();
  *node_ids = get_bb_data().keys.data();
}


void
get(size_t *count, Bounding_box **box, const bool world)
{
  *count = get_bb_data().keys.size();

  if(world)
  {
    *box = get_bb_data().world_bb.data();
  }
  else
  {
    *box = get_bb_data().local_bb.data();
  }
}


void
get(const Node &node, Bounding_box &out, const bool world)
{
  get_data_helper(
    node,
    out,
    get_bb_data().keys.data(),
    get_bb_data().keys.size(),
    (uintptr_t)&world,
    
    // Found
    [](const size_t index, Bounding_box &out, uintptr_t user_data)
    {
      const bool world = *(bool*)user_data;
      
      if(world)
      {
        out = get_bb_data().world_bb[index];
      }
      else
      {
        out = get_bb_data().local_bb[index];
      }
    },
    
    // Not Found
    [](const uint32_t node_id, Bounding_box &out, uintptr_t user_data)
    {
      LOG_ERROR("Failed to find data")
    }
  );
}


void
remove_bounding_box(Node &node)
{
  remove_data_helper(
    node,
    get_bb_data().keys.data(),
    get_bb_data().keys.size(),
    get_bb_data().type_id,
    0,
    
    // Remove
    [](const size_t index, uintptr_t user_data)
    {
      get_bb_data().keys.erase(index);
      get_bb_data().world_bb.erase(index);
      get_bb_data().local_bb.erase(index);
//      get_bb_data().actions.erase(index);
    },
    
    // Not found
    [](const uint32_t node_id, uintptr_t user_data)
    {
      LOG_ERROR("Failed to find data");
    }
  );
}


void
set(Node &node, const Bounding_box &in)
{
  set_data_helper(
    node,
    in,
    get_bb_data().keys.data(),
    get_bb_data().keys.size(),
    get_bb_data().type_id,
    (uintptr_t)&node,
    
    // Updated existing data
    [](const size_t index, const Bounding_box &in, uintptr_t user_data)
    {
      const Node *node = reinterpret_cast<Node*>(user_data);
      
      Nil::Data::Transform trans;
      Nil::Data::get(*node, trans);
      
      const math::vec3 pos = math::vec3_init(trans.position);
      const math::vec3 min = math::vec3_add(math::vec3_init(in.min), pos);
      const math::vec3 max = math::vec3_add(math::vec3_init(in.max), pos);
      
      Nil::Data::Bounding_box world_in = in;
      memcpy(world_in.min, min.data, sizeof(world_in.min));
      memcpy(world_in.max, max.data, sizeof(world_in.max));
    
      get_bb_data().local_bb[index] = in;
      get_bb_data().world_bb[index] = world_in;
    },
    
    // Set new data
    [](const uint32_t node_id, const Bounding_box &in, uintptr_t user_data)
    {
      Node node(node_id);
      
      Nil::Data::Transform trans;
      Nil::Data::get(node, trans);
      
      const math::vec3 pos = math::vec3_init(trans.position);
      const math::vec3 min = math::vec3_add(math::vec3_init(in.min), pos);
      const math::vec3 max = math::vec3_add(math::vec3_init(in.max), pos);
      
      Nil::Data::Bounding_box world_in = in;
      memcpy(world_in.min, min.data, sizeof(world_in.min));
      memcpy(world_in.max, max.data, sizeof(world_in.max));
      
      uint32_t *key_arr = get_bb_data().keys.data();
      
      get_bb_data().local_bb.emplace_back(in);
      get_bb_data().world_bb.emplace_back(world_in);
      get_bb_data().keys.emplace_back(node_id);
    }
  );
}


// ----------------------------------------------------------------- [ Info ] --


bool
has_bounding_box(const Node &node)
{
  return find_node(
    node,
    get_bb_data().keys.data(),
    get_bb_data().keys.size()
  );
}


bool
has(const Node &node, const Bounding_box &)
{
  return has_bounding_box(node);
}


uint64_t
get_type_id(const Bounding_box &)
{
  return get_bb_data().type_id;
}


const char*
get_type_name(const Bounding_box &in)
{
  return "BoundingBox";
}


size_t
bounding_box_count()
{
  return get_bb_data().keys.size();
}


// --------------------------------------------------------------- [ Events ] --


void
events(const uint32_t data, size_t *count, Bounding_box_data **out_data, Node **out_node)
{
  LIB_ASSERT(false);
}


} // ns
} // ns
