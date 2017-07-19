#include <roa/vector3.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --
  

Vector3::Vector3()
: Vector3(0.f, 0.f, 0.f)
{
}


Vector3::Vector3(const float x, const float y, const float z)
: data{x, y, z}
{
}


Vector3::Vector3(const float *data)
: Vector3(data[0], data[1], data[2])
{
}


} // ns
