#include <aspect/roa_aspect.hpp>
#include <roa/roa.hpp>
#include <nil/task.hpp>
#include <nil/aspect.hpp>
#include <lib/assert.hpp>
#include <nil/data/logic.hpp>


namespace Nil_ext {
namespace ROA_Aspect {


// ------------------------------------------------- [ ROA Aspect Interface ] --


void
start_up(Nil::Engine &engine, Nil::Aspect &aspect)
{
  
}


void
events(Nil::Engine &engine, Nil::Aspect &aspect)
{
  
  // Logic Task
  {
    // No need to check if data exists. 99.99999999% of the time it does.
    
    Nil::Task::cpu_task(
      Nil::Task::CPU::EARLY_THINK,
      aspect.user_data,
      logic_update_callback
    );
  }
}


// ------------------------------------------------------------ [ ROA Tasks ] --


void
logic_start_callback(Nil::Engine &engine, uintptr_t user_data)
{

}


void
logic_update_callback(Nil::Engine &engine, uintptr_t user_data)
{
  Data *self = reinterpret_cast<Data*>(user_data);
  LIB_ASSERT(self);
  
  size_t logic_count = 0;
  Nil::Data::Logic *logic = nullptr;
  
  Nil::Data::get(&logic_count, &logic);
  
  for(size_t i = 0; i < logic_count; ++i)
  {
    using update_fn = void(*)(ROA::Object obj);
    
    if(logic[i].update_func)
    {
      ((update_fn)(logic[i].update_func))(ROA::Object((uint32_t)logic[i].user_data));
    }
  }
}



} // ns
} // ns
