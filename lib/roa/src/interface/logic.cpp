#include <roa/logic.hpp>
#include <common/common.hpp>
#include <nil/node.hpp>
#include <nil/data/logic.hpp>
#include <lib/assert.hpp>



namespace ROA {


// ------------------------------------------------------------- [ Lifetime ] --


Logic::Logic()
{
  Nil::Data::Logic data{};
  
  Nil::Node node = ROA_detail::get_node(*this);
  Nil::Data::set(node, data);
}


Logic::Logic(const ROA_nullptr null)
: Object(null)
{
}


Logic::~Logic()
{
}


// ---------------------------------------------------------------- [ Event ] --


void
Logic::send_message(const uint32_t id, const uintptr_t msg_data)
{
  Nil::Data::Logic data{};
  Nil::Node node = ROA_detail::get_node(*this);
  
  if(Nil::Data::has(node, data))
  {
    Nil::Data::get(node, data);
    
    if(data.message_func)
    {
      ((Logic_message_func)(data.message_func))(*this, id, msg_data);
    }
  }
}



// ----------------------------------------------------------- [ Attributes ] --


void
Logic::set_update_func(Logic_update_func update_fn)
{
  Nil::Data::Logic data{};
  Nil::Node node = ROA_detail::get_node(*this);
  
  if(Nil::Data::has(node, data))
  {
    Nil::Data::get(node, data);
  }
  
  data.logic_id  = 1;
  data.user_data = (uintptr_t)get_instance_id();
  data.update_func = (uintptr_t)update_fn;
  
  Nil::Data::set(node, data);
}


void
Logic::set_message_func(Logic_message_func msg_fn)
{
  Nil::Data::Logic data{};
  Nil::Node node = ROA_detail::get_node(*this);
  
  if(Nil::Data::has(node, data))
  {
    Nil::Data::get(node, data);
  }
  
  data.logic_id  = 1;
  data.user_data = (uintptr_t)get_instance_id();
  data.message_func  = (uintptr_t)msg_fn;
  
  Nil::Data::set(node, data);
}


} // ns
