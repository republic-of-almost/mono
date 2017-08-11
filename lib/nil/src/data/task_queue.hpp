#ifndef TASK_QUEUE_INCLUDED_2BA00441_1AD8_42FE_80D9_6A5E3538451C
#define TASK_QUEUE_INCLUDED_2BA00441_1AD8_42FE_80D9_6A5E3538451C


#include <nil/task.hpp>
#include <lib/array.hpp>


namespace Nil {
namespace Data {


struct Cpu_task
{
  Nil::Task::Cpu_task_func func;
  uintptr_t user_data;
};


struct Gpu_task
{
  Nil::Task::Gpu_task_func func;
  uintptr_t user_data;
};


struct Task_queues
{
  // CPU Tasks
  lib::array<Cpu_task> early_think;
  lib::array<Cpu_task> think;
  lib::array<Cpu_task> late_think;
  
  // GPU Tasks
  lib::array<Gpu_task> pre_render;
  lib::array<Gpu_task> render;
  lib::array<Gpu_task> post_render;
};
  

Task_queues&
get_task_queues();


} // ns
} // ns


#endif // inc guard
