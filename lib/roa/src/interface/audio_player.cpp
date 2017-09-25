#include <roa/audio_player.hpp>
#include <roa/audio_source.hpp>
#include <common/common.hpp>
#include <nil/node.hpp>
#include <nil/data/audio.hpp>


namespace ROA {


Audio_player::Audio_player()
//: Audio_player(Audio_source{})
{
}


Audio_player::Audio_player(const ROA_nullptr null)
: ROA::Object(null)
{
}


Audio_player::~Audio_player()
{
}


//Audio_player::Audio_player(const Audio_source &src)
//: Object()
//{
//  Nil::Data::Audio data{};
//  
//  data.audio_id = src.get_id();
//  data.volume = 1.f;
//  
//  Nil::Node node = ROA_detail::get_node(*this);
//  Nil::Data::set(node, data);
//}


void
Audio_player::set_source(Audio_source &in)
{
  if(!in)
  {
    return;
  }

  Nil::Node node = ROA_detail::get_node(*this);
  
  Nil::Data::Audio data{};
  Nil::Data::get(node, data);
  
  data.audio_id = in.get_id();
  
  Nil::Data::set(node, data);
}


Audio_source
Audio_player::get_source() const
{
  Nil::Node node = ROA_detail::get_node(*this);
  
  Nil::Data::Audio data{};
  Nil::Data::get(node, data);
  
//  return Audio_source(data.audio_id);
  return Audio_source{};
}


void
Audio_player::set_volume(const float val)
{
  Nil::Node node = ROA_detail::get_node(*this);
  
  Nil::Data::Audio data{};
  Nil::Data::get(node, data);
  
  data.volume = val;
  
  Nil::Data::set(node, data);
}


float
Audio_player::get_volume() const
{
  Nil::Node node = ROA_detail::get_node(*this);
  
  Nil::Data::Audio data{};
  Nil::Data::get(node, data);
  
  return data.volume;
}


void
Audio_player::set_play(const bool play)
{
  Nil::Node node = ROA_detail::get_node(*this);
  
  Nil::Data::Audio data{};
  Nil::Data::get(node, data);
  
  data.request_state = play ? Nil::Data::Audio::PLAY : Nil::Data::Audio::STOP;
  
  Nil::Data::set(node, data);
}


} // ns
