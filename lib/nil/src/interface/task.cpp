#include <nil/task.hpp>
#include <data/task_queue.hpp>



namespace Nil {
namespace Task {


void
cpu_task(CPU queue, uintptr_t user_data, Cpu_task_func func)
{
  if(func)
  {
    switch(queue)
    {
      case(CPU::EARLY_THINK):
        Nil::Data::get_task_queues().early_think.emplace_back(func, user_data);
        return;
        
      case(CPU::THINK):
        Nil::Data::get_task_queues().think.emplace_back(func, user_data);
        return;
        
      case(CPU::LATE_THINK):
        Nil::Data::get_task_queues().late_think.emplace_back(func, user_data);
        return;
    };
  }
}


void
gpu_task(GPU queue, uintptr_t user_data, Gpu_task_func func)
{
  if(func)
  {
    switch(queue)
    {
      case(GPU::PRE_RENDER):
        Nil::Data::get_task_queues().pre_render.emplace_back(func, user_data);
        return;
        
      case(GPU::RENDER):
        Nil::Data::get_task_queues().render.emplace_back(func, user_data);
        return;
        
      case(GPU::POST_RENDER):
        Nil::Data::get_task_queues().post_render.emplace_back(func, user_data);
        return;
    };
  }
}


} // ns
} // ns
