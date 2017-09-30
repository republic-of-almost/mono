#ifndef ASPECT_INCLUDED_CAAB0E57_5E24_4643_9C26_95880895F3B4
#define ASPECT_INCLUDED_CAAB0E57_5E24_4643_9C26_95880895F3B4


#include <nil/fwd.hpp>


typedef void(*aspect_callback_fn)(Nil_ctx *ctx, void *self);


struct Nil_aspect
{

};


namespace Nil {


/*
  Callback prototypes.
*/
using aspect_startup_fn     = void(*)(Engine &engine, Aspect &aspect);
using aspect_tick_fn        = void(*)(Engine &engine, Aspect &aspect);

using aspect_shutdown_fn    = void(*)(Engine &engine, Aspect &aspect);
using aspect_ui_window      = void(*)(Engine &engine, Aspect &aspect);
using aspect_ui_menu        = void(*)(Engine &engine, Aspect &aspect);

// --

struct Aspect
{
  aspect_startup_fn         start_up_fn;
  aspect_tick_fn            tick_fn;

  aspect_shutdown_fn        shutdown_fn;
  aspect_ui_window          ui_window_fn;
  aspect_ui_menu            ui_menu_fn;
  
  bool                      want_to_quit;
  uint64_t                  data_types;
  uintptr_t                 user_data;
};


} // ns


#endif // inc guard
