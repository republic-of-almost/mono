#ifndef AUDIO_PLAYER_INCLUDED_D30C39CC_7F71_4D48_B2AB_3B8B7F9A57F9
#define AUDIO_PLAYER_INCLUDED_D30C39CC_7F71_4D48_B2AB_3B8B7F9A57F9


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


namespace ROA {


class Audio_player : public Object
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --


  explicit          Audio_player();
  explicit          Audio_player(const Audio_source &src);
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  void              set_source(Audio_source &in);
  Audio_source      get_source() const;
  
  
  void              set_volume(const float vol);
  float             get_volume() const;
  
  
  void              play();
};


} // ns



#endif // inc guard
