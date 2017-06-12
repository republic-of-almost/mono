#include <nil/node_event.hpp>
#include <graph/graph_data.hpp>


namespace Nil {


Event_list::Event_list(std::vector<Event_data> evts)
: pointer(0)
, events(evts)
{
  
}


Event_list::Event_list()
: pointer(0)
{
  
}


Event_list::Event_list(const Event_list &other)
{
  pointer = other.pointer;
  events = other.events;
}


void
Event_list::reset()
{
  pointer = 0;
}


bool
Event_list::get(Event_data &data)
{
  if(pointer >= events.size())
  {
    return false;
  }
  
  data = events[pointer++];
  
  return true;
}


namespace Event {


Node
node(const Event_data &evt)
{
  return Node(evt.node_id, false);
}


bool
node_added(const Event_data &evt)
{
  return !!(evt.events & Graph::Event::ADDED);
}


bool
node_moved(const Event_data &evt)
{
  return !!(evt.events & Graph::Event::MOVED);
}


bool
node_removed(const Event_data &evt)
{
  return !!(evt.events & Graph::Event::REMOVED);
}


bool
node_updated(const Event_data &evt)
{
  return !!(evt.events & Graph::Event::UPDATED_DATA);
}


} // ns
} // ns
