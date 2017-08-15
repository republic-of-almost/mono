#include <roa/light.hpp>
#include <roa/color.hpp>
#include <common/common.hpp>
#include <lib/color.hpp>


namespace ROA {


Light::Light()
: Object()
{
  Nil::Data::Light data{};
  
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::set(node, data);
}


Light::~Light()
{
}


Color
Light::get_color() const
{
  Nil::Data::Light data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  return Color(data.color[0], data.color[1], data.color[2], 255);
}


void
Light::set_color(const Color &color)
{
  Nil::Data::Light data{};
  ROA_detail::get_nil_data(*this, data);
  
  lib::rgba col = lib::color::init(color.get_uint());
  data.color[0] = lib::color::get_channel_1i(col);
  data.color[1] = lib::color::get_channel_1i(col);
  data.color[2] = lib::color::get_channel_1i(col);
  
  ROA_detail::set_nil_data(*this, data);
}


Light_type
Light::get_type() const
{
  Nil::Data::Light data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  return data.type == Nil::Data::Light::POINT ? Light_type::POINT : Light_type::DIRECTIONAL;
}


void
Light::set_type(const Light_type type)
{
  Nil::Data::Light data{};
  ROA_detail::get_nil_data(*this, data);
  
  data.type = type == Light_type::POINT ? Nil::Data::Light::POINT : Nil::Data::Light::DIR;
  
  ROA_detail::set_nil_data(*this, data);
}


float
Light::get_constant_atten() const
{
  Nil::Data::Light data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  return data.atten_const;
}


void
Light::set_constant_atten(const float atten)
{
  Nil::Data::Light data{};
  ROA_detail::get_nil_data(*this, data);
  
  data.atten_const = atten;
  
  ROA_detail::set_nil_data(*this, data);
}


float
Light::get_exponential_atten() const
{
  Nil::Data::Light data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  return data.atten_exponential;
}


void
Light::set_exponential_atten(const float atten)
{
  Nil::Data::Light data{};
  ROA_detail::get_nil_data(*this, data);
  
  data.atten_exponential = atten;
  
  ROA_detail::set_nil_data(*this, data);
}


float
Light::get_linear_atten() const
{
  Nil::Data::Light data{};
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::get(node, data);
  
  return data.atten_linear;
}


void
Light::set_linear_atten(const float atten)
{
  Nil::Data::Light data{};
  ROA_detail::get_nil_data(*this, data);
  
  data.atten_linear = atten;
  
  ROA_detail::set_nil_data(*this, data);
}


void
Light::set_atten(const float con, const float lin, const float exp)
{
  Nil::Data::Light data{};
  ROA_detail::get_nil_data(*this, data);
  
  data.atten_const = con;
  data.atten_linear = lin;
  data.atten_exponential = exp;
  
  ROA_detail::set_nil_data(*this, data);
}



} // ns
