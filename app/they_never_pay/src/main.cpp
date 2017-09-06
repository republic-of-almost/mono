#include <roa/roa.hpp>

#include <lib/logging.hpp>
#include <lib/memory.hpp>

#include <game/game_manager.hpp>
#include <data/data.hpp>


/*
  Use this for any testing, fires at the end of the frame.
*/
void
custom_tick(uintptr_t user_data)
{
}


/*
  Entry Point
*/
int
main()
{
//  lib::mem::alloc<float, lib::mem::tag::NUMBER, 256>();

  lib::logging::set_output(lib::logging::out::file | lib::logging::out::console);
  
  ROA::Application app;
  
  Game_data::setup();
//  Game_data::load_assets();

  ROA::Object game;
  game.add_component<Game::Game_manager>();

  /*
    Run Game
  */
  app.run(custom_tick, 0);

  return 0;
}
