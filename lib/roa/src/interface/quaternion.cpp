#include <roa/quaternion.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --
  

Quaternion::Quaternion()
: Quaternion(0.f, 0.f, 0.f, 1.f)
{
}


Quaternion::Quaternion(const float *data)
: Quaternion(data[0], data[1], data[2], data[3])
{
}


Quaternion::Quaternion(const float x, const float y, const float z, const float w)
: data{x, y, z, w}
{
}


} // ns
