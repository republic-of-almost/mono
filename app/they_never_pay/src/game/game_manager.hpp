/*!

  Game
  --
  Monitors the game state. Decides when the game should start/end etc.

*/
#ifndef GAME_INCLUDED_9185C149_04BE_4EA3_9241_D1705052C374
#define GAME_INCLUDED_9185C149_04BE_4EA3_9241_D1705052C374


#include <roa/roa.hpp>


namespace Game {


struct Game_manager : ROA::Component
{
  static constexpr uint32_t get_rtti() { return 235; }
  
  
  // -------------------------------------------------------------- [ Hooks ] --
  
  
  void on_start() override;
  void on_think() override;
  
  
  // ------------------------------------------------------------ [ Members ] --
  
  
  ROA::Object this_obj{nullptr}; // temp until components have an object.
  ROA::Object scene{nullptr};
  ROA::Object actor{nullptr};
  
};


} // ns


#endif // inc guard
