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

#include <roa/roa.hpp>

int
main()
{
  lib::logging::set_output(lib::logging::out::console);

  ROA::Application app;

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
  app.run([](uintptr_t user_data){
    // No custom tick
  });

  return 0;
}
