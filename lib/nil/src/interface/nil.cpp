#include <nil/nil.hpp>
#include <nil/node.hpp>
#include <nil/node_event.hpp>
#include <nil/aspect.hpp>
#include <data/data.hpp>
#include <graph/graph.hpp>
#include <graph/graph_data.hpp>
#include <lib/array.hpp>
#include <lib/assert.hpp>


namespace Nil {


struct Engine::Impl
{
  std::vector<Aspect> aspects;
  
  Engine_settings settings;
  std::vector<Event_data> pending_events;
};


Engine::Engine()
: m_impl(new Impl)
{
  LIB_ASSERT(m_impl);
  
  if(!m_impl)
  {
    LOG_ERROR_ONCE("Engine is in corrupted state.");
    return;
  }
  
  m_impl->settings.pause_node_events = false;
}


Engine::~Engine()
{
  delete m_impl;
  m_impl = nullptr;
}


void
Engine::add_aspect(Nil::Aspect aspect)
{
  LIB_ASSERT(m_impl);
  
  if(!m_impl)
  {
    LOG_ERROR_ONCE("Engine is in corrupted state.");
    return;
  }
  
  m_impl->aspects.emplace_back(aspect);
  
  if(aspect.start_up_fn)
  {
    aspect.start_up_fn(*this, m_impl->aspects.back());
  }
}


size_t
Engine::aspect_count() const
{
  LIB_ASSERT(m_impl);
  
  if(!m_impl)
  {
    LOG_ERROR_ONCE("Engine is in corrupted state.");
    return false;
  }

  return m_impl->aspects.size();
}


bool
Engine::run()
{
  LIB_ASSERT(m_impl);
  
  if(!m_impl)
  {
    LOG_ERROR_ONCE("Engine is in corrupted state.");
    return false;
  }
  
  /*
    Create list of events
  */
  {
    m_impl->pending_events.clear();
    
    Graph::Data *graph = Data::get_graph_data();
    LIB_ASSERT(graph);
  
    for(size_t j = 0; j < graph->node_events.size(); ++j)
    {
      m_impl->pending_events.emplace_back(
        Event_data{
          graph->node_events[j].node_id,
          graph->node_events[j].event_action
        }
      );
    }
  }
 
  // Distro Events
  if(!m_impl->settings.pause_node_events)
  {
    std::vector<Event_data> nodes;
  
    for(Aspect &asp : m_impl->aspects)
    {
      const size_t event_count = Data::get_graph_data()->node_events.size();
      
      for(size_t j = 0; j < event_count; ++j)
      {
        const Node event_node(Data::get_graph_data()->node_events[j].node_id);
        const uint64_t data_types = asp.data_types;

        if(data_types & event_node.get_data_type_id())
        {
          const uint32_t actions = Data::get_graph_data()->node_events[j].event_action;
          
          nodes.emplace_back(Event_data{event_node.get_id(), actions});
        }
      }
    
      if(asp.events_fn)
      {
        Event_list evt_list(nodes);
        asp.events_fn(*this, asp, evt_list);
      }
    }
    
    Graph::think(Data::get_graph_data());
  }
  
  // Thinking
  {
    for(Aspect &asp : m_impl->aspects)
    {
      if(asp.early_think_fn)
      {
        asp.early_think_fn(*this, asp);
      }
    }
    
    // Think
    for(Aspect &asp : m_impl->aspects)
    {
      if(asp.think_fn)
      {
        asp.think_fn(*this, asp);
      }
    }
    
    // Late Think
    for(Aspect &asp : m_impl->aspects)
    {
      if(asp.late_think_fn)
      {
        asp.late_think_fn(*this, asp);
      }
    }
  }
  
  /*
    Check to see if any aspects are ready to quit.
  */
  {
    bool should_quit = false;
    
    for(Aspect &asp : m_impl->aspects)
    {
      should_quit |= asp.want_to_quit;
    }

    return !should_quit;
  }
}


void
Engine::set_settings(const Engine_settings &in)
{
  m_impl->settings = in;
}


void
Engine::get_settings(Engine_settings &out)
{
  out = m_impl->settings;
}


void
Engine::get_state(Engine_state &out)
{
  const Graph::Data *graph = Data::get_graph_data();
  const Graph::Component_data *data = &graph->component_data;
  
  /*
    Events
  */
  out.node_events = m_impl->pending_events.data();
  out.node_event_count  = m_impl->pending_events.size();
  
  /*
    Set the count of things.
  */
  out.node_count              = Graph::node_descendants_count(graph, 0);
  out.bounding_box_count      = out.node_count;
  out.camera_count            = data->camera_node_id.size();
  out.collider_count          = data->collider_node_id.size();
  out.developer_count         = data->developer_node_id.size();
  out.gamepad_count           = data->gamepad_node_id.size();
  out.graphics_count          = data->graphics_node_id.size();
  out.keyboard_count          = data->keyboard_node_id.size();
  out.light_count             = data->light_node_id.size();
  out.logic_count             = data->logic_node_id.size();
  out.material_count          = data->material_node_id.size();
  out.mesh_count              = data->mesh_node_id.size();
  out.mesh_resource_count     = data->mesh_resource_node_id.size();
  out.mouse_count             = data->mouse_node_id.size();
  out.resouce_count           = data->resource_node_id.size();
  out.rigidbody_count         = data->rigidbody_node_id.size();
  out.texture_count           = data->texture_node_id.size();
  out.texture_resource_count  = data->texture_resource_node_id.size();
  out.transform_count         = out.node_count;
  out.window_count            = data->window_node_id.size();
}


// ------------------------------------------------------- [ Debugging Info ] --


size_t
Engine::graph_data_count() const
{
  return Data::get_graph_data()->node_id.size();
}


const uint32_t*
Engine::graph_data_get_ids() const
{
  return Data::get_graph_data()->node_id.data();
}


const uint64_t*
Engine::graph_data_details() const
{
  return Data::get_graph_data()->parent_depth_data.data();
}


const math::transform*
Engine::graph_data_local_transforms() const
{
  return Data::get_graph_data()->local_transform.data();
}


const math::transform*
Engine::graph_data_world_transforms() const
{
  return Data::get_graph_data()->world_transform.data();
}


} // ns
