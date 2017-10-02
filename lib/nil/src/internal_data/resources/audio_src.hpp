#ifndef AUDIO_SRC_INCLUDED_229468BE_F238_484F_8E31_B9AA630C4387
#define AUDIO_SRC_INCLUDED_229468BE_F238_484F_8E31_B9AA630C4387


#include <nil/resource/audio.hpp>
#include <lib/array.hpp>


struct Audio_data
{
  lib::array<uint32_t, 128>             keys{uint32_t{}};
  lib::array<Nil::Resource::Audio, 128> audio{Nil::Resource::Audio{}};
};


#endif // inc guard
