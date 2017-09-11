#ifndef AUDIO_INCLUDED_390E3EFC_CBA6_4281_B9A2_D54A0478D7AF
#define AUDIO_INCLUDED_390E3EFC_CBA6_4281_B9A2_D54A0478D7AF


#include <roa/fundamental.hpp>
#include <roa/resource.hpp>


namespace ROA {


class Audio final : public Resource
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  explicit          Audio();
  explicit          Audio(const uint32_t id);
  explicit          Audio(const char *filename);
  

};


} // ns


#endif // inc guard
