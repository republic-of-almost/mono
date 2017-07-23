#include <roa/mouse.hpp>
#include <roa/point.hpp>
#include <common/common.hpp>
#include <nil/data/mouse.hpp>


namespace ROA {
namespace Mouse {


Point
get_delta()
{
  const Nil::Node app_node = ROA_detail::get_application_node();
  Nil::Data::Mouse data{};
  Nil::Data::get(app_node, data);
  
  return Point{ data.delta[0],data.delta[1] };
}


Point
get_position()
{
  const Nil::Node app_node = ROA_detail::get_application_node();
  Nil::Data::Mouse data{};
  Nil::Data::get(app_node, data);
  
  return Point{ (float)data.position[0], (float)data.position[1] };
}


void
set_captured(const bool set)
{
  Nil::Node app_node = ROA_detail::get_application_node();
  Nil::Data::Mouse data{};
  Nil::Data::get(app_node, data);
  
  data.capture = set;
  Nil::Data::set(app_node, data);
}


bool
is_captured()
{
  Nil::Node app_node = ROA_detail::get_application_node();
  Nil::Data::Mouse data{};
  Nil::Data::get(app_node, data);
  
  return data.capture;
}


} // ns
} // ns
