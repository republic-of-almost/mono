#include <game/game.hpp>
#include <game/actor.hpp>


namespace Game {


void
Game_manager::on_start()
{
  // -- Create this -- //
  {
    this_obj = ROA::Object{};
    this_obj.set_name("Game Manager");
  }
  
  // -- Load Scene -- //
  {
    scene = ROA::Object{};
    scene.set_name("Game Scene");
  }

  // -- Create the Actor -- //
  {
    actor = ROA::Object{};
    actor.add_component<Actor>();
    actor.set_name("Actor!");
  }
}


void
Game_manager::on_think()
{
  
}


} // ns
