#include <game/dynamic_object.hpp>
#include <lib/assert.hpp>



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
  obj->entity = ROA::Object();
}


void
send_message(ROA::Object obj, uint32_t id, uintptr_t data)
{
  Dynamic_object *self = reinterpret_cast<Dynamic_object*>(obj.get_user_data());
  LIB_ASSERT(self);

  self->start = lib::timer::get_current_time();
  self->end   = lib::timer::get_current_time() + 10000;
  self->time  = self->time;
}


namespace {

inline float
linear(const float time)
{
  return time * time;
}

}


void
update(ROA::Object obj)
{
  Dynamic_object *self = reinterpret_cast<Dynamic_object*>(obj.get_user_data());
  LIB_ASSERT(self);
  
  if(self->start)
  {
    self->time = lib::timer::get_current_time();
    
    lib::milliseconds length = self->end - self->start;
    const float t = float(self->time - self->start) / float(length);
    const float tt = 5.5f;//linear(t);
    
    ROA::Transform trans = obj.get_transform();
    trans.set_scale(ROA::Vector3(tt,tt,tt));
  }
}


} // ns
