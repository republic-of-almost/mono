#include <lib/logging.hpp>

#include <game/static_objects.hpp>
#include <game/actor.hpp>
#include <game/game.hpp>
#include <data/data.hpp>

#include <lib/memory.hpp>

#include <nau/nau.h>
#include <nau/renderers/opengl3.h>

#include <roa/roa.hpp>
#include <stdio.h>



struct TestScript : public ROA::Component
{
  static constexpr uint32_t get_rtti() { return 123; }
  
  void on_think() override
  {
    
  }
};


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
  Game_data::load_assets();

  ROA::Object game;
  game.add_component<Game::Game_manager>();

//  ROA::Object actor;
//  actor.add_component<Game::Actor>();

//  Game::Actor actor;
//  Game::setup(&actor);
  
  /*
    Run Game
  */
  app.run(custom_tick, 0);

  return 0;
}
