#include <game/game_manager.hpp>
#include <game/actor_kinematic.hpp>


namespace Game {


void
Game_manager::on_start()
{
  // -- This -- //
  {
    ROA::Object this_obj = get_object();
    this_obj.set_name("Game Manager");
  }
  
  // -- Load Scene -- //
  {
    scene = ROA::Object{};
    scene.set_name("Game Scene");
    ROA::Scene::load(scene, "mesh/test_level.gltf");
    
    ROA::Transform trans = scene.get_transform();
    trans.set_scale(ROA::Vector3(5.f, 5.f, 5.f));
  }

  // -- Create the Actor -- //
  {
    actor = ROA::Object{};
    actor.add_component<Actor_kinematic>();
    actor.set_name("Actor");
  }
  
}


void
Game_manager::on_think()
{
  // -- Game Conditions -- //
}


} // ns
