#include <roa/camera.hpp>
#include <roa/color.hpp>
#include <common/common.hpp>
#include <nil/node.hpp>
#include <nil/data/camera.hpp>
#include <lib/logging.hpp>
#include <math/general/constants.hpp>


namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --
  

Camera::Camera()
: Object()
{
  Nil::Data::Camera data{};
  data.width              = 1.f;
  data.height             = 1.f;
  data.near_plane         = 0.1f;
  data.far_plane          = 1000.f;
  data.fov                = math::tau() * 0.125f;
  data.clear_depth_buffer = true;
  data.clear_color_buffer = true;
  
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::set(node, data);
}


Camera::Camera(const ROA_nullptr null)
: Object(null)
{
}


Camera::~Camera()
{
}


// ----------------------------------------------------------- [ Attributes ] --


int32_t
Camera::get_priority() const
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  
  return data.priority;
}


void
Camera::set_priority(const int32_t priority)
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  data.priority = priority;
  ROA_detail::set_nil_data(*this, data);
}


float
Camera::get_field_of_view() const
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  
  return data.fov;
}


void
Camera::set_field_of_view(const float radians)
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  data.fov = radians;
  ROA_detail::set_nil_data(*this, data);
}


float
Camera::get_width() const
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  
  return data.width;
}


void
Camera::set_width(const float width)
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  data.width = width;
  ROA_detail::set_nil_data(*this, data);
}


float
Camera::get_height() const
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  
  return data.height;
}


void
Camera::set_height(const float height)
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  data.height = height;
  ROA_detail::set_nil_data(*this, data);
}


float
Camera::get_near_plane() const
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  
  return data.near_plane;
}


void
Camera::set_near_plane(const float near)
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  data.near_plane = near;
  ROA_detail::set_nil_data(*this, data);
}


float
Camera::get_far_plane() const
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  
  return data.far_plane;
}


void
Camera::set_far_plane(const float far)
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  data.far_plane = far;
  ROA_detail::set_nil_data(*this, data);
}


Color
Camera::get_clear_color() const
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  
  return Color(data.clear_color);
}


void
Camera::set_clear_color(const Color &color)
{
  Nil::Data::Camera data{};
  ROA_detail::get_nil_data(*this, data);
  data.clear_color = color.get_uint();
  ROA_detail::set_nil_data(*this, data);
}

  
} // ns
