#include <roa/vector3.hpp>
#include <math/vec/vec3.hpp>


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


// ----------------------------------------------------------- [ Attributes ] --


Vector3
Vector3::add(const Vector3 &other) const
{
  return Vector3(
    math::vec3_add(
      math::vec3_init(get_data()),
      math::vec3_init(other.get_data())
    ).data
  );
}


Vector3
Vector3::scale(const float scale) const
{
  return Vector3(
    math::vec3_scale(
      math::vec3_init(get_data()),
      scale
    ).data
  );
}


} // ns
