#include <roa/keyboard.hpp>
#include <common/common.hpp>


namespace ROA {
namespace Keyboard {


uint32_t
key_state(const uint32_t key)
{
  const Nil::Node app_node = ROA_detail::get_application_node();
  Nil::Data::Keyboard data{};
  Nil::Data::get(app_node, data);
  
  if(data.key_state)
  {
    return data.key_state[key];
  }
  
  return ROA::KeyState::UP;
}


const char *
key_name(const uint32_t key)
{
  return "";
}


} // ns
} // ns
