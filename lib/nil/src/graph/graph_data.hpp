#ifndef GRAPH_DATA_INCLUDED_9B0564E7_3925_432D_B08E_4F5C35A8DCCE
#define GRAPH_DATA_INCLUDED_9B0564E7_3925_432D_B08E_4F5C35A8DCCE


#include <lib/utilities.hpp>
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


/*
  TODO:
  Break this up, have data register with the graph.
  This means we could expose it to add any type of data.
*/
struct Component_data
{
  lib::array<uint32_t>                    audio_node_id;
  lib::array<Nil::Data::Audio>            audio_data;
  
  lib::array<uint32_t>                    audio_resource_node_id;
  lib::array<Nil::Data::Audio_resource>   audio_resource_data;

  lib::array<uint32_t>                    camera_node_id;
  lib::array<Nil::Data::Camera>           camera_data;
  
  lib::array<uint32_t>                    collider_node_id;
  lib::array<Nil::Data::Collider>         collider_data;

  lib::array<uint32_t>                    developer_node_id;
  lib::array<Nil::Data::Developer>        developer_data;

  lib::array<uint32_t>                    gamepad_node_id;
  lib::array<Nil::Data::Gamepad>          gamepad_data;

  lib::array<uint32_t>                    graphics_node_id;
  lib::array<Nil::Data::Graphics>         graphics_data;
  
  lib::array<uint32_t>                    keyboard_node_id;
  lib::array<Nil::Data::Keyboard>         keyboard_data;

  lib::array<uint32_t>                    light_node_id;
  lib::array<Nil::Data::Light>            light_data;

  lib::array<uint32_t>                    logic_node_id;
  lib::array<Nil::Data::Logic>            logic_data;

  lib::array<uint32_t>                    material_node_id;
  lib::array<Nil::Data::Material>         material_data;
  
  lib::array<uint32_t>                    mesh_node_id;
  lib::array<Nil::Data::Mesh>             mesh_data;

  lib::array<uint32_t>                    mesh_resource_node_id;
  lib::array<Nil::Data::Mesh_resource>    mesh_resource_data;

  lib::array<uint32_t>                    mouse_node_id;
  lib::array<Nil::Data::Mouse>            mouse_data;
 
  lib::array<uint32_t>                    resource_node_id;
  lib::array<Nil::Data::Resource>         resource_data;

  lib::array<uint32_t>                    rigidbody_node_id;
  lib::array<Nil::Data::Rigidbody>        rigidbody_data;
  
  lib::array<uint32_t>                    texture_node_id;
  lib::array<Nil::Data::Texture>          texture_data;

  lib::array<uint32_t>                    texture_resource_node_id;
  lib::array<Nil::Data::Texture_resource> texture_resource_data;
  
  lib::array<uint32_t>                    window_node_id;
  lib::array<Nil::Data::Window>           window_data;
};


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

  // -- Data -- //
  Component_data              component_data;

  // -- Other -- //
  uint64_t                    graph_tick;
  uint32_t                    instance_counter;
};



} // ns
} // ns


#endif // inc guard
