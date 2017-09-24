#ifndef COLOR_INCLUDED_8F28E809_CA42_48F6_87FC_AD0CC5AF11C9
#define COLOR_INCLUDED_8F28E809_CA42_48F6_87FC_AD0CC5AF11C9


#include <roa/fundamental.hpp>


namespace ROA {


class Color final
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --


  explicit          Color();
  explicit          Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
  explicit          Color(const uint32_t hex);
  explicit          Color(const float rgba[]);
  explicit          Color(const uint8_t rgba[]);
  
  
  // ---------------------------------------------------------- [ Operators ] --
  
  
  bool              operator==(const Color &other) const;
  bool              operator!=(const Color &other) const;
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  uint32_t          get_uint() const;
  
  
private:


  uint32_t          m_data;

};


} // ns


#endif // inc guard
