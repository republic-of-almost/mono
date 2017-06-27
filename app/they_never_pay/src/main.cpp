#include <nil/nil.hpp>
#include <aspect/aspects.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <lib/logging.hpp>
#include <lib/string.hpp>

#include <game/static_objects.hpp>
#include <game/actor.hpp>

#include <data/data.hpp>

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <lib/memory.hpp>


void
app_tick()
{
  #ifdef __EMSCRIPTEN__
  nil_engine.run();
  #endif
}

int
main()
{
  Nil::Engine nil_engine;
  Nil_ext::load_aspects(nil_engine);
  
  Game_data::setup();
  Game_data::load_assets();

  Game::Actor actor;
  Game::setup(&actor);
  
  Game::Static_objects objs;
  Game::setup(&objs);
  
  /*
    Run Game
  */
  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(app_tick, -1, 1);
  #else
  while(nil_engine.run())
  {
    app_tick();
  }
  #endif

  return 0;
}
