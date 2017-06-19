#ifndef GRAPH_FWD_INCLUDED_7713EBEE_139A_4B55_B942_8BDA0EC3A342
#define GRAPH_FWD_INCLUDED_7713EBEE_139A_4B55_B942_8BDA0EC3A342


#include <stdint.h>


namespace Nil {


class Node; // fwd for callback prototype.
using graph_tick_fn = void(*)(uintptr_t user_data);
using node_delete_fn = void(*)(const uint32_t node, uintptr_t user_data);
using data_dependecy_alert_fn = void(*)(const uint32_t node, uintptr_t user_data);


namespace Graph {


struct Data;
struct Event;


} // ns
} // ns


#endif // inc guard
