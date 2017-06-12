#ifndef ASPECT_INCLUDED_CAAB0E57_5E24_4643_9C26_95880895F3B4
#define ASPECT_INCLUDED_CAAB0E57_5E24_4643_9C26_95880895F3B4


#include <nil/fwd.hpp>
#include <stddef.h>
#include <stdint.h>


namespace Nil {


/*
  Callback prototypes.
*/
using aspect_startup_fn     = void(*)(Engine &engine, Aspect &aspect);
using aspect_node_events_fn = void(*)(Engine &engine, Aspect &aspect, Event_list &event_list);
using aspect_early_think_fn = void(*)(Engine &engine, Aspect &aspect);
using aspect_think_fn       = void(*)(Engine &engine, Aspect &aspect);
using aspect_late_think_fn  = void(*)(Engine &engine, Aspect &aspect);
using aspect_shutdown_fn    = void(*)(Engine &engine, Aspect &aspect);
using aspect_ui_window      = void(*)(Engine &engine, Aspect &aspect);
using aspect_ui_menu        = void(*)(Engine &engine, Aspect &aspect);

// --

struct Aspect
{
  aspect_startup_fn         start_up_fn;
  aspect_node_events_fn     events_fn;
  aspect_early_think_fn     early_think_fn;
  aspect_think_fn           think_fn;
  aspect_late_think_fn      late_think_fn;
  aspect_shutdown_fn        shutdown_fn;
  aspect_ui_window          ui_window_fn;
  aspect_ui_menu            ui_menu_fn;
  
  bool                      want_to_quit;
  
  uint64_t                  data_types;
  
  uintptr_t                 user_data;
};


} // ns


#endif // inc guard
