#ifndef GRAPH_DATA_INCLUDED_C75347EC_83FC_47B2_89DF_8F641A0DD327
#define GRAPH_DATA_INCLUDED_C75347EC_83FC_47B2_89DF_8F641A0DD327


#include <roa_lib/fundamental.h>
#include <roa_math/math.h>
#include <roa_lib/atomic.h>


struct short_string
{
  char data[NIL_MAX_NODE_NAME_LENGTH];
};


struct node_data
{
  uintptr_t user_data;
  uint64_t node_type_id;
  uint64_t tags;
  short_string name;
};


// ----------------------------------------------------------------- [ Data ] --


struct Data
{
  // -- Graph -- //
  /*
  * We store common attributes for all nodes.
  */
  lib::array<uint32_t>        node_id;                  // Unique instance IDs.
  lib::array<uint64_t>        parent_depth_data;        // Parent ID and Depth.
  lib::array<math::transform> local_transform;          // Local.
  lib::array<math::transform> world_transform;          // World.
  lib::array<node_data>       data;


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



#endif /* inc guard */