#include <roa/quaternion.hpp>
#include <roa/vector3.hpp>
#include <math/quat/quat.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --
  

Quaternion::Quaternion()
: Quaternion(0.f, 0.f, 0.f, 1.f)
{
}


Quaternion::Quaternion(const Vector3 &axis, const float angle)
: Quaternion(
    math::quat_init_with_axis_angle(
      axis.get_x(),
      axis.get_y(),
      axis.get_z(),
      angle
      ).data
    )
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
