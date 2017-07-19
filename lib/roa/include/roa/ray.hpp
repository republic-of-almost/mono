#ifndef RAY_INCLUDED_F76939C9_4A90_4FAF_BCCC_420E17CDDC17
#define RAY_INCLUDED_F76939C9_4A90_4FAF_BCCC_420E17CDDC17


#include <roa/fundamental.hpp>
#include <roa/vector3.hpp>


namespace ROA {


class Ray
{
public:

  explicit    Ray(const Vector3 &start, const Vector3 &end);
  
  Vector3     get_start();
  Vector3     get_end();
  
private:

  Vector3     m_start;
  Vector3     m_end;

};


} // ns


#endif // inc guard
