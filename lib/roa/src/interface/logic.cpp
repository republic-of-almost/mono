#include <roa/logic.hpp>
#include <common/common.hpp>
#include <nil/node.hpp>
#include <nil/data/logic.hpp>
#include <lib/assert.hpp>


namespace {


void
update_logic_wrapper(const uint32_t id, uintptr_t user_data)
{
	LIB_ASSERT(user_data);
  LIB_ASSERT(id);
  
  ROA::Logic_update_func func = (ROA::Logic_update_func)user_data;
  func(ROA::Object(id));
}


} // anon ns


namespace ROA {


Logic::Logic()
{
}


Logic::~Logic()
{
}


void
Logic::update_func(Logic_update_func update_fn)
{
  Nil::Data::Logic data{};
  Nil::Node node = ROA_detail::get_node(*this);
  
  if(Nil::Data::has(node, data))
  {
    Nil::Data::get(node, data);
  }
  
  data.logic_id  = 1;
  data.user_data = (uintptr_t)update_fn;
  data.think_01  = update_logic_wrapper;
  
  Nil::Data::set(node, data);
}


} // ns
