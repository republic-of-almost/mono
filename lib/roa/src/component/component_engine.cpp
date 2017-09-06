#include <component/component_engine.hpp>
#include <roa/component.hpp>
#include <lib/array.hpp>


namespace {


lib::array<uint64_t> keys;
lib::array<ROA::Component*> components;
lib::array<ROA::Component*> start;


} // anon ns


namespace ROA_detail {


// ------------------------------------------------------ [ Component Hooks ] --


void
components_start_hooks()
{
  for(auto &comp : start)
  {
    //comp->on_start();
  }
  
  start.clear();
}


void
components_early_think_hooks()
{
  for(auto &comp : components)
  {
    comp->on_early_think();
  }
}


void
components_think_hooks()
{
  for(auto &comp : components)
  {
    comp->on_think();
  }
}


void
components_late_think_hooks()
{
  for(auto &comp : components)
  {
    comp->on_late_think();
  }
}


// ---------------- [ Public Interface - roa/detail/component_interface.hpp ] --


bool
add_component(const uint32_t object_id, ROA::Component *comp)
{
  components.emplace_back(comp);
  start.emplace_back(comp);
  comp->on_start();
  
  return true;
}

  
bool
has_component(const uint32_t object_id, const uint32_t rtti_id)
{
  return false;
}


ROA::Component*
get_component(const uint32_t object_id, const uint32_t rtti_id)
{
  for(ROA::Component *comp : components)
  {
    if(comp->get_object().get_instance_id() == object_id)
    {
      return comp;
    }
  }
  
  return nullptr;
}


} // ns
