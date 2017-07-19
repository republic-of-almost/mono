#include <roa/transform.hpp>
#include <roa/vector3.hpp>
#include <roa/quaternion.hpp>
#include <common/common.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Transform::Transform()
: Object()
{
}


// ----------------------------------------------------------- [ Attributes ] --


Vector3
Transform::get_position() const
{
  Nil::Data::Transform data{};
  ROA_detail::get_nil_data(*this, data);

  return Vector3(data.position);
}


void
Transform::set_position(const ROA::Vector3 &in)
{
  Nil::Data::Transform data{};
  ROA_detail::get_nil_data(*this, data);
  
  data.position[0] = in.get_x();
  data.position[1] = in.get_y();
  data.position[2] = in.get_z();
  
  ROA_detail::set_nil_data(*this, data);
}


Vector3
Transform::get_scale() const
{
  Nil::Data::Transform data{};
  ROA_detail::get_nil_data(*this, data);

  return Vector3(data.scale);
}


void
Transform::set_scale(const Vector3 &in)
{
  Nil::Data::Transform data{};
  ROA_detail::get_nil_data(*this, data);
  
  data.scale[0] = in.get_x();
  data.scale[1] = in.get_y();
  data.scale[2] = in.get_z();
  
  ROA_detail::set_nil_data(*this, data);
}


Quaternion
Transform::get_rotation() const
{
  Nil::Data::Transform data{};
  ROA_detail::get_nil_data(*this, data);

  return Quaternion(data.rotation);
}


void
Transform::set_rotation(const Quaternion &in)
{
  Nil::Data::Transform data{};
  ROA_detail::get_nil_data(*this, data);
  
  data.rotation[0] = in.get_x();
  data.rotation[1] = in.get_y();
  data.rotation[2] = in.get_z();
  data.rotation[3] = in.get_w();
  
  ROA_detail::set_nil_data(*this, data);
}


Vector3
Transform::get_forward() const
{
  return Vector3();
}


Vector3
Transform::get_up() const
{
  return Vector3();
}


Vector3
Transform::get_left() const
{
  return Vector3();
}


} // ns
