#include <nil/node_controller.hpp>
#include <nil/node.hpp>
#include <nil/node_event.hpp>
#include <data/data.hpp>
#include <graph/graph_data.hpp>
#include <lib/utilities.hpp>


namespace Nil {


// ------------------------------------------------------------- [ Node List] --


struct Node_list::Impl
{
  uint64_t frame_tick;
  Node *begin;
  Node *end;
};


Node_list::Node_list(Node *begin, Node *end)
: m_impl(new Impl{
  Data::get_graph_data()->graph_tick,
  begin,
  end,
})
{
  LIB_ASSERT(m_impl);
}


Node_list::Node_list(const Node_list &other)
: m_impl(new Impl{
  other.m_impl->frame_tick,
  other.m_impl->begin,
  other.m_impl->end,
})
{
}


Node_list&
Node_list::operator=(const Node_list &other)
{
  m_impl->frame_tick = other.m_impl->frame_tick;
  m_impl->begin = other.m_impl->begin;
  m_impl->end = other.m_impl->end;
  
  return *this;
}


Node
Node_list::operator[](const size_t i)
{
  if(size() > i)
  {
    return *(m_impl->begin + i);
  }
  
  return Node(nullptr);
}


Node_list::~Node_list()
{
  LIB_ASSERT(m_impl);
  delete m_impl;
}


namespace {

inline bool
node_list_is_valid_frame(const uint64_t frame_tick)
{
  return frame_tick >= Data::get_graph_data()->graph_tick;
}

} // anon ns

  
size_t
Node_list::size() const
{
  LIB_ASSERT(m_impl);
  return node_list_is_valid_frame(m_impl->frame_tick) ?
    (m_impl->end - m_impl->begin) :
    0;
}


Node*
Node_list::begin() const
{
  LIB_ASSERT(m_impl);
  return node_list_is_valid_frame(m_impl->frame_tick) ? m_impl->begin : nullptr;
}


Node*
Node_list::end() const
{
  LIB_ASSERT(m_impl);
  return node_list_is_valid_frame(m_impl->frame_tick) ? m_impl->end : nullptr;
}


// ------------------------------------------------------ [ Node Controller ] --


struct Node_controller::Impl
{
  uint64_t data_types;
  std::vector<Nil::Node> removed_nodes;
  std::vector<Nil::Node> updated_nodes;
  std::vector<Nil::Node> added_nodes;
  std::vector<Nil::Node> updated_and_added;
  std::vector<Nil::Node> all_nodes;
};


Node_controller::Node_controller(const uint64_t data_types)
: m_impl(new Impl{
  data_types,
  {},
  {},
  {},
  {},
  {}
})
{
}


Node_controller::~Node_controller()
{
  delete m_impl;
}


Node_controller::Node_controller(Node_controller &&other) noexcept
: m_impl(new Impl{
  other.m_impl->data_types,
  static_cast<decltype(other.m_impl->removed_nodes)&&>(other.m_impl->removed_nodes),
  static_cast<decltype(other.m_impl->updated_nodes)&&>(other.m_impl->updated_nodes),
  static_cast<decltype(other.m_impl->added_nodes)&&>(other.m_impl->added_nodes),
  static_cast<decltype(other.m_impl->updated_and_added)&&>(other.m_impl->updated_and_added),
  static_cast<decltype(other.m_impl->all_nodes)&&>(other.m_impl->all_nodes),
})
{
}


Node_controller&
Node_controller::operator=(Node_controller &&other) noexcept
{
  m_impl->data_types        = other.m_impl->data_types;
  m_impl->removed_nodes     = static_cast<decltype(other.m_impl->removed_nodes)&&>(other.m_impl->removed_nodes);
  m_impl->removed_nodes     = static_cast<decltype(other.m_impl->updated_nodes)&&>(other.m_impl->updated_nodes);
  m_impl->added_nodes       = static_cast<decltype(other.m_impl->added_nodes)&&>(other.m_impl->added_nodes);
  m_impl->updated_and_added = static_cast<decltype(other.m_impl->updated_and_added)&&>(other.m_impl->updated_and_added);
  m_impl->all_nodes         = static_cast<decltype(other.m_impl->all_nodes)&&>(other.m_impl->all_nodes);
  
  return *this;
}


void
Node_controller::process(Event_list &evts)
{
  LIB_ASSERT(m_impl);
  evts.reset();
  
  /*
    Clear old data.
  */
  m_impl->removed_nodes.clear();
  m_impl->added_nodes.clear();
  m_impl->updated_nodes.clear();
  m_impl->updated_and_added.clear();
  
  Event_data evt;
  
  while(evts.get(evt))
  {
    Node node(evt.node_id);
  
    if(m_impl->data_types & node.get_data_type_id())
    {
      /*
        Nodes that are removed shouldn't bother letting aspects create data
        that they just have to delete.
      */
      if(!Event::node_removed(evt))
      {
        if(Event::node_added(evt))
        {
          m_impl->added_nodes.emplace_back(node);
          m_impl->updated_and_added.emplace_back(node);
          m_impl->all_nodes.emplace_back(node);
        }

        if(Event::node_moved(evt) || Event::node_updated(evt))
        {
          m_impl->updated_nodes.emplace_back(node);
          m_impl->updated_and_added.emplace_back(node);
        }
      }
      else if(Event::node_removed(evt))
      {
        m_impl->removed_nodes.emplace_back(node);
        
        auto it = m_impl->all_nodes.begin();
        auto end = m_impl->all_nodes.end();
        
        for(; it != end; ++it)
        {
          if(*it == node)
          {
            m_impl->all_nodes.erase(it);
            break;
          }
        }
      }
    }
  }
}


Node_list
Node_controller::added() const
{
  LIB_ASSERT(m_impl);
  return Node_list(
    &m_impl->added_nodes.front(),
    &m_impl->added_nodes.back() + 1
  );
}


Node_list
Node_controller::updated() const
{
  LIB_ASSERT(m_impl);
  return Node_list(
    &m_impl->updated_nodes.front(),
    &m_impl->updated_nodes.back() + 1
  );
}


Node_list
Node_controller::updated_and_added() const
{
  LIB_ASSERT(m_impl);
  return Node_list(
    &m_impl->updated_and_added.front(),
    &m_impl->updated_and_added.back() + 1
  );
}


Node_list
Node_controller::removed() const
{
  LIB_ASSERT(m_impl);
  return Node_list(
    &m_impl->removed_nodes.front(),
    &m_impl->removed_nodes.back() + 1
  );
}


Node_list
Node_controller::all() const
{
  LIB_ASSERT(m_impl);
  return Node_list(
    &m_impl->all_nodes.front(),
    &m_impl->all_nodes.back() + 1
  );
}


} // ns
