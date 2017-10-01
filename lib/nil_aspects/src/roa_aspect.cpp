#include <aspect/roa_aspect.hpp>
#include <roa/roa.hpp>
#include <nil/task.hpp>
#include <nil/aspect.hpp>
#include <lib/assert.hpp>
#include <nil/data/logic.hpp>


namespace Nil_ext {
namespace ROA_Aspect {


/* ------------------------------------------------------ [ ROA Lifetime ] -- */


void
start_up(Nil_ctx *ctx, void *data)
{
  
}


void
events(Nil_ctx *ctx, void *data)
{
  
  // Logic Task
  {
    // No need to check if data exists. 99.99999999% of the time it does.
    
    nil_task_cpu_add(
      ctx,
      NIL_CPU_TASK_EARLY_THINK,
      logic_update_callback,
      data
    );
  }
}


void
shut_down(Nil_ctx *ctx, void *data)
{
}


/* --------------------------------------------------------- [ ROA Tasks ] -- */


void
logic_start_callback(Nil_ctx *ctx, void *data)
{

}


void
logic_update_callback(Nil_ctx *ctx, void *data)
{
  Data *self = reinterpret_cast<Data*>(data);
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
