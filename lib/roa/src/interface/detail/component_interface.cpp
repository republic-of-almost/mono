#include <roa/detail/component_interface.hpp>


namespace ROA_detail {


bool
add_component(const uint32_t object_id, ROA::Component *comp)
{
  return  false;
}

  
bool
has_component(const uint32_t object_id, const uint32_t rtti_id)
{
  return false;
}


ROA::Component*
get_component(const uint32_t object_id, const uint32_t rtti_id)
{
  return nullptr;
}


} // ns
