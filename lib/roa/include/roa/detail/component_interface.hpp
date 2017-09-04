#ifndef COMPONENT_INTERFACE_INCLUDED_F0F83D3B_3846_47C2_B376_BAA6B29BC7F6
#define COMPONENT_INTERFACE_INCLUDED_F0F83D3B_3846_47C2_B376_BAA6B29BC7F6


#include <roa/fundamental.hpp>


namespace ROA_detail {


bool
add_component(const uint32_t object_id, ROA::Component *comp);

  
bool
has_component(const uint32_t object_id, const uint32_t rtti_id);


ROA::Component*
get_component(const uint32_t object_id, const uint32_t rtti_id);


} // ns


#endif // inc guard
