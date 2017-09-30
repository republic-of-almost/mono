#ifndef TASK_INCLUDED_A1087C62_BA4D_4992_8A72_E3E163AB8317
#define TASK_INCLUDED_A1087C62_BA4D_4992_8A72_E3E163AB8317


#include <nil/fwd.hpp>






namespace Nil {
namespace Task {


/*
  CPU Task
  --
  This task pool, will execute each queue in order until its empty.
  then more to the next queue.
  
  When Late think is empty the next frame will start.
  start.
*/
enum class CPU
{
  EARLY_THINK,
  THINK,
  LATE_THINK,
};


using Cpu_task_func = void(*)(Nil::Engine &engine, uintptr_t user_data);


/*
  Call this in the aspect tick update to queue your task.
*/
void
cpu_task(CPU queue, uintptr_t user_data, Cpu_task_func func);


/*
  GPU Task
  --
  This task pool will execute PRE_PRENDER in a loop until a render task exists.
  it will then procede through RENDER and POST_RENDER queues.
*/

enum class GPU
{
  PRE_RENDER,
  RENDER,
  POST_RENDER,
};


using Gpu_task_func = void(*)(Nil::Engine &engine, uintptr_t user_data);

/*
  Call this in the aspect tick update to queue your task.
*/
void
gpu_task(GPU queue, uintptr_t user_data, Gpu_task_func func);


}
} // ns


#endif // inc guard
