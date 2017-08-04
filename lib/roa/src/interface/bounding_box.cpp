#include <roa/bounding_box.hpp>
#include <roa/vector3.hpp>
#include <common/common.hpp>
#include <nil/data/bounding_box.hpp>
#include <nil/node.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Bounding_box::Bounding_box()
: Bounding_box(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f))
{
}


Bounding_box::Bounding_box(const Vector3 &min, const Vector3 &max)
: Object()
{
  Nil::Data::Bounding_box data{};
  
  data.min[0] = min.get_x();
  data.min[1] = min.get_y();
  data.min[2] = min.get_z();
  
  data.max[0] = max.get_x();
  data.max[1] = max.get_y();
  data.max[2] = max.get_z();
  
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::set(node, data);
}


// ----------------------------------------------------------- [ Attributes ] --


Vector3
Bounding_box::get_min() const
{
  Nil::Data::Bounding_box data{};
  ROA_detail::get_nil_data(*this, data);
  
  return Vector3(data.min);
}


void
Bounding_box::set_min(const Vector3 &in)
{
  Nil::Data::Bounding_box data{};
  ROA_detail::get_nil_data(*this, data);
  data.min[0] = in.get_x();
  data.min[1] = in.get_y();
  data.min[2] = in.get_z();
  ROA_detail::set_nil_data(*this, data);
}


Vector3
Bounding_box::get_max() const
{
  Nil::Data::Bounding_box data{};
  ROA_detail::get_nil_data(*this, data);
  
  return Vector3(data.max);
}


void
Bounding_box::set_max(const Vector3 &in)
{
  Nil::Data::Bounding_box data{};
  ROA_detail::get_nil_data(*this, data);
  data.max[0] = in.get_x();
  data.max[1] = in.get_y();
  data.max[2] = in.get_z();
  ROA_detail::set_nil_data(*this, data);
}


} // ns
