#ifndef NODE_EVENT_INCLUDED_B745B084_85D4_4A3E_9CD0_A81EFF9C7C60
#define NODE_EVENT_INCLUDED_B745B084_85D4_4A3E_9CD0_A81EFF9C7C60


#include <nil/node.hpp>
#include <stdint.h>

#include <vector>

namespace Nil {


struct Event_data
{
  uint32_t  node_id;
  uint32_t  events;  
};


class Event_list
{
private:

  friend class Engine;

  explicit    Event_list(std::vector<Event_data> events);

public:

  explicit    Event_list();
              Event_list(const Event_list &other);
  
  void        reset();
  
  bool        get(Event_data &data);
  
private:

  size_t pointer;
  std::vector<Event_data> events;

};



namespace Event {


/*!
  Get Node
*/
Node
node(const Event_data &evt);


/*!
  Returns true if this node has been added to the graph
*/
bool
node_added(const Event_data &evt);


/*!
  Returns true if this node was moved in the graph.
*/
bool
node_moved(const Event_data &evt);


/*!
  Returns true if this node has been remvoed from the graph.
*/
bool
node_removed(const Event_data &evt);


/*!
  Returns true if this node's data has been updated.
*/
bool
node_updated(const Event_data &evt);


} // ns
} // ns


#endif // inc guard
