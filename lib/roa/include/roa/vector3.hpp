#ifndef VECTOR3_INCLUDED_17E996D5_A00B_425E_AD53_01BF0A55217E
#define VECTOR3_INCLUDED_17E996D5_A00B_425E_AD53_01BF0A55217E


#include <roa/fundamental.hpp>


namespace ROA {


class Vector3 final
{
public:


  // ----------------------------------------------------------- [ Lifetime ] --
  
  
  explicit            Vector3();
  explicit            Vector3(const float x, const float y, const float z);
  explicit            Vector3(const float *data);
  
  
  // --------------------------------------------------------- [ Attributes ] --


  inline float        get_x() const { return data[0]; };
  inline float        get_y() const { return data[1]; };
  inline float        get_z() const { return data[2]; };
  
  inline const float* get_data() const  { return data; }
  inline float*       get_data()        { return data; }

  Vector3             add(const Vector3 &other) const;
  Vector3             scale(const float scale) const;

private:


  float             data[3];

};


} // ns


#endif // inc guard
