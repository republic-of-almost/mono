#include <roa/audio_player.hpp>
#include <roa/audio_source.hpp>


namespace ROA {


Audio_player::Audio_player()
: Audio_player(Audio_source{})
{
}


Audio_player::Audio_player(const Audio_source &src)
{
}


void
Audio_player::set_source(Audio_source &in)
{
}


Audio_source
Audio_player::get_source() const
{
  return Audio_source{};
}


void
Audio_player::set_volume(const float val)
{
}


float
Audio_player::get_volume() const
{
  return 0.f;
}


} // ns
