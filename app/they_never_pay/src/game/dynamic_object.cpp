#include <game/dynamic_object.hpp>



namespace Game {


namespace {


inline void
think(ROA::Object node)
{
  
}


Dynamic_object objs[16];
size_t curr_obj = 0;


} // ns


Dynamic_object *
get_dyn_obj()
{
  if(curr_obj < 16)
  {
    return &objs[curr_obj++];
  }
  
  return nullptr;
}


void
setup(Dynamic_object *obj)
{

}


void
send_message(ROA::Object obj, uint32_t id, uintptr_t data)
{
  ROA::Transform t = obj.get_transform();
  t.set_position(t.get_position().add(ROA::Vector3(0,1,0)));
}


void
update(ROA::Object obj)
{
  
}


} // ns
