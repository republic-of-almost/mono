#ifndef LIGHT_INCLUDED_B49A5854_D775_4BFD_AD6B_2E23FE27E5D5
#define LIGHT_INCLUDED_B49A5854_D775_4BFD_AD6B_2E23FE27E5D5


#include <roa/fundamental.hpp>
#include <roa/object.hpp>


namespace ROA {


enum class Light_type
{
  POINT,
  DIRECTIONAL,
};


class Light final : public Object
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  explicit        Light();
  explicit        Light(const ROA_nullptr null);
                  ~Light();
  
  
  // --------------------------------------------------------- [ Attributes ] --

  
//  Color           get_color() const;
//  void            set_color(const Color &color);
  
  Light_type      get_type() const;
  void            set_type(const Light_type type);
  
  float           get_constant_atten() const;
  void            set_constant_atten(const float atten);
  
  float           get_exponential_atten() const;
  void            set_exponential_atten(const float atten);
  
  float           get_linear_atten() const;
  void            set_linear_atten(const float atten);
  
  void            set_atten(const float con, const float lin, const float exp);
  
};


} // ns


#endif // inc guard
