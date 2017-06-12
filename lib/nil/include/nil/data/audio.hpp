#ifndef AUDIO_INCLUDED_D5CEC090_0E28_4A12_BC48_4860E1ED16FB
#define AUDIO_INCLUDED_D5CEC090_0E28_4A12_BC48_4860E1ED16FB


#include <nil/fwd.hpp>
#include <stdint.h>


namespace Nil {
namespace Data {


struct Audio
{
  uint32_t audio_id;
  uint32_t channel_id;
  
  enum { NO_REQ_STATE, PLAY, STOP }         request_state;
  enum { NO_CURR_STATE, PLAYING, STOPPED }   current_state;
  
  bool loop;
  
  float volume;
};


void
get(const Node &node, Audio &out);


void
set(Node &node, const Audio &in);


bool
has_audio(const Node &node);


uint64_t
get_type_id(const Audio &in);


} // ns
} // ns


#endif // inc guard
