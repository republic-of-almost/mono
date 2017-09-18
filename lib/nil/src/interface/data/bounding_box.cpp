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
          Nil::Node node(id);
          
          Nil::Data::Bounding_box in = data->local_bb[index];
          
          Nil::Data::Transform trans;
          Nil::Data::get(node, trans);
          
          // -- Scale -- //
          
          const math::vec3 scale = math::vec3_init(trans.world_scale);
          constexpr size_t count = 8;
          
          const float *max = in.max;
          const float *min = in.min;
          
          const math::vec3 corners[count]
          {
            math::vec3_multiply(scale, math::vec3_init(min)),
            math::vec3_multiply(scale, math::vec3_init(max[0], min[1], min[2])),
            math::vec3_multiply(scale, math::vec3_init(min[0], min[1], max[2])),
            math::vec3_multiply(scale, math::vec3_init(max[0], min[1], max[2])),

            math::vec3_multiply(scale, math::vec3_init(max)),
            math::vec3_multiply(scale, math::vec3_init(min[0], max[1], max[2])),
            math::vec3_multiply(scale, math::vec3_init(max[0], max[1], min[2])),
            math::vec3_multiply(scale, math::vec3_init(min[0], max[1], min[2])),
          };
          
          // -- Rotate -- //
          
          const math::quat rot = math::quat_init(trans.world_rotation);
          
          math::vec3 rot_corners[count];
          
          for(int i = 0; i < count; ++i)
          {
            rot_corners[i] = math::quat_rotate_point(rot, corners[i]);
          }
          
          // -- Find new min max -- //
          
          float new_min[3] {
            math::float_max(),math::float_max(),math::float_max()
          };
          
          float new_max[3] {
            math::float_min(),math::float_min(),math::float_min()
          };
          
          for(auto &rot_pt : rot_corners)
          {
            math::max(new_max, new_max, rot_pt.data, 3);
            math::min(new_min, new_min, rot_pt.data, 3);
          }
          
          // -- Translate -- //
          
          math::add(new_min, new_min, trans.world_position, 3);
          math::add(new_max, new_max, trans.world_position, 3);
          
          // -- Save -- //
          
          Nil::Data::Bounding_box world_in = in;
          memcpy(world_in.min, new_min, sizeof(world_in.min));
          memcpy(world_in.max, new_max, sizeof(world_in.max));
        
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
