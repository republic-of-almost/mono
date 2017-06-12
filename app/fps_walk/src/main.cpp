#include <nil/nil.hpp>
#include <aspect/aspects.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <lib/logging.hpp>
#include <lib/string.hpp>

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include <game/data.hpp>
#include <game/ground.hpp>
#include <game/environment.hpp>
#include <game/actor.hpp>


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
  lib::logging::set_output(lib::logging::out::console);
  
  Nil::Engine nil_engine;
  Nil_ext::load_aspects(nil_engine);

  Game_data::setup();

  Game::Ground ground_entity;
  Game::setup(&ground_entity);
  
  Game::Environment environment_entity;
  Game::setup(&environment_entity);
  
  Game::Actor actor_entity;
  Game::setup(&actor_entity);

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
