#include <internal_data/task_queue.hpp>
#include <internal_data/internal_data.hpp>


void
nil_task_cpu_process(Nil_ctx *ctx, const Nil_cpu_task *tasks, const size_t count)
{
  for(size_t i = 0; i < count; ++i)
  {
    if(tasks[i].callback)
    {
      tasks[i].callback(ctx, tasks[i].user_data);
    }
  }
}


void
nil_task_gpu_process(Nil_ctx *ctx, const Nil_gpu_task *tasks, const size_t count)
{
  for(size_t i = 0; i < count; ++i)
  {
    if(tasks[i].callback)
    {
      tasks[i].callback(ctx, tasks[i].user_data);
    }
  }
}
