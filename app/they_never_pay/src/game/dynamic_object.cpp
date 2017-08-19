#include <game/dynamic_object.hpp>
#include <lib/assert.hpp>



namespace Dynamic_object {


namespace {


Data objs[16];
size_t curr_obj = 0;


} // ns


Data *
get_dyn_obj()
{
  if(curr_obj < 16)
  {
    return &objs[curr_obj++];
  }
  
  return nullptr;
}


void
setup(Data *obj)
{
  obj->entity = ROA::Object();
}


void
message_handler(ROA::Object obj, uint32_t id, uintptr_t data)
{
  Data *self = reinterpret_cast<Data*>(obj.get_user_data());
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
  Data *self = reinterpret_cast<Data*>(obj.get_user_data());
  LIB_ASSERT(self);
  
  if(self->start)
  {
    self->time = lib::timer::get_current_time();
    
    lib::milliseconds length = self->end - self->start;
    const float t = float(self->time - self->start) / float(length);
    const float tt = linear(t);
    
    ROA::Transform trans = obj.get_transform();
    trans.set_position(ROA::Vector3(trans.get_position().get_x(), tt, trans.get_position().get_z()));
  }
}


} // ns
