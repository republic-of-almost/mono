#ifndef QUATERNION_INCLUDED_7B01C7AE_4716_42D9_8240_9E35BC8CD4A0
#define QUATERNION_INCLUDED_7B01C7AE_4716_42D9_8240_9E35BC8CD4A0


#include <roa/fundamental.hpp>


namespace ROA {


class Quaternion final
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  

  explicit        Quaternion();
  explicit        Quaternion(const Vector3 &axis, const float angle);
  explicit        Quaternion(const float *data);
  explicit        Quaternion(const float x,
                             const float y,
                             const float z,
                             const float w);
  
  
  // --------------------------------------------------------- [ Attributes ] --
  
  
  inline float    get_x() const { return data[0]; }
  inline float    get_y() const { return data[1]; }
  inline float    get_z() const { return data[2]; }
  inline float    get_w() const { return data[3]; }


private:

  float           data[4];

};


} // ns


#endif // inc guard
