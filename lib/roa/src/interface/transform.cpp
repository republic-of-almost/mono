#include <roa/transform.hpp>
#include <roa/vector3.hpp>
#include <roa/quaternion.hpp>
#include <common/common.hpp>
#include <math/math.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Transform::Transform()
: Object()
{
}


Transform::Transform(const ROA_nullptr null)
: Object(null)
{
}


Transform::Transform(
  const Vector3 &pos,
  const Vector3 &scale,
  const Quaternion &rot
)
: Transform()
{
  set_position(pos);
  set_scale(scale);
  set_rotation(rot);
}


// ----------------------------------------------------------- [ Attributes ] --


Vector3
Transform::get_world_position() const
{
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  return Vector3(data.world_position);
}


void
Transform::set_world_position(const Vector3 pos)
{
  Nil::Node this_node = ROA_detail::get_node(*this);
  Nil::Data::Transform this_data{};
  Nil::Data::get(this_node, this_data);
  
  // -- Calculate world difference -- //
  
  // Position
  {
    math::vec3 this_pos = math::vec3_init(this_data.world_position);
    math::vec3 that_pos = math::vec3_init(pos.get_data());
    
    math::vec3 diff_pos = math::vec3_subtract(that_pos, this_pos);
    
    memcpy(this_data.position, diff_pos.data, sizeof(this_data.position));
    
    Nil::Data::set(this_node, this_data);
  }
}


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


Vector3
Transform::get_world_scale() const
{
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  return Vector3(data.world_scale);
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


Quaternion
Transform::get_world_rotation() const
{
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  return Quaternion(data.world_rotation);
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
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  const math::transform trans = math::transform_init(
    math::vec3_init(data.position),
    math::vec3_init(data.scale),
    math::quat_init(data.rotation)
  );
  
  const math::vec3 dir = math::transform_fwd(trans);
  
  return Vector3(dir.data);
}


Vector3
Transform::get_world_forward() const
{
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  const math::transform trans = math::transform_init(
    math::vec3_init(data.world_position),
    math::vec3_init(data.world_scale),
    math::quat_init(data.world_rotation)
  );
  
  const math::vec3 dir = math::transform_fwd(trans);
  
  return Vector3(dir.data);
}


Vector3
Transform::get_up() const
{
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  const math::transform trans = math::transform_init(
    math::vec3_init(data.position),
    math::vec3_init(data.scale),
    math::quat_init(data.rotation)
  );
  
  const math::vec3 dir = math::transform_up(trans);
  
  return Vector3(dir.data);
}


Vector3
Transform::get_world_up() const
{
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  const math::transform trans = math::transform_init(
    math::vec3_init(data.world_position),
    math::vec3_init(data.world_scale),
    math::quat_init(data.world_rotation)
  );
  
  const math::vec3 dir = math::transform_up(trans);
  
  return Vector3(dir.data);
}


Vector3
Transform::get_left() const
{
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  const math::transform trans = math::transform_init(
    math::vec3_init(data.position),
    math::vec3_init(data.scale),
    math::quat_init(data.rotation)
  );
  
  const math::vec3 dir = math::transform_left(trans);
  
  return Vector3(dir.data);
}


Vector3
Transform::get_world_left() const
{
  Nil::Data::Transform data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  const math::transform trans = math::transform_init(
    math::vec3_init(data.world_position),
    math::vec3_init(data.world_scale),
    math::quat_init(data.world_rotation)
  );
  
  const math::vec3 dir = math::transform_left(trans);
  
  return Vector3(dir.data);
}


} // ns
