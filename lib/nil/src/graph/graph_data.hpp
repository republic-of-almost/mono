#ifndef GRAPH_DATA_INCLUDED_9B0564E7_3925_432D_B08E_4F5C35A8DCCE
#define GRAPH_DATA_INCLUDED_9B0564E7_3925_432D_B08E_4F5C35A8DCCE


#include <lib/utilities.hpp>
#include "graph_fwd.hpp"
#include <math/math.hpp>
#include <stdint.h>

#include <nil/data/data.hpp>


namespace Nil {
namespace Graph {


struct short_string
{
  char data[16];
};


struct Event
{
  enum {
    ADDED         = 1 << 0,
    REMOVED       = 1 << 1,
    MOVED         = 1 << 2,
    UPDATED_DATA  = 1 << 3,
  };
  
  uint32_t        event_action;
  
  uint32_t        node_id;
  
  uint32_t        parent;
  short_string    name;
  math::transform transform;
  math::aabb      boundinb_box;
  uint64_t        node_type_id;
  uintptr_t       user_data;
  uint64_t        last_update;
};


// ----------------------------------------------------------------- [ Data ] --


struct Data
{
  // -- Graph -- //
  /*
   * We store common attributes for all nodes.
   */
  
  lib::array<uint32_t>        node_id;            // Unique instance IDs.
  lib::array<uint64_t>        parent_depth_data;  // Parent ID and Depth.
  lib::array<short_string>    name;               // Name
  lib::array<math::transform> local_transform;    // Local.
  lib::array<math::transform> world_transform;    // World.
  lib::array<math::aabb>      bounding_box;       // Bounding box.
  lib::array<uint64_t>        node_type_id;       // Node type id.
  lib::array<uintptr_t>       user_data;          // External extra data.
  lib::array<uint64_t>        last_update;        // graph_tick was updated.
  
  // -- Events -- //
  lib::array<Event>           node_events;        // When changes happen.
  
  struct graph_type
  {
    graph_tick_fn           tick_cb;
    node_delete_fn          delete_cb;
    data_dependecy_alert_fn dependency_cb;
    uintptr_t               user_data;
    uint64_t                type_id;
    uint64_t                dependency;
  };
  
  lib::array<graph_type>    graph_type_data;

  // -- Other -- //
  uint64_t                    graph_tick;
  uint32_t                    instance_counter;
  
  // -- Callbacks -- //
  
  struct graph_tick_cb {
    graph_tick_fn fn;
    uintptr_t user_data;
  };
  lib::array<graph_tick_cb>               frame_tick_callbacks;
  
  // -- //
  
  struct node_delete_cb {
    node_delete_fn fn;
    uintptr_t user_data;
  };
  lib::array<node_delete_cb>              node_delete_callbacks;
  
};



} // ns
} // ns


#endif // inc guard
