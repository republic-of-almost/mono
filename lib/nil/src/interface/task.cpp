#include <nil/task.hpp>
#include <data/task_queue.hpp>
#include <lib/logging.hpp>
#include <lib/assert.hpp>


void
nil_task_cpu_add(
  Nil_ctx *ctx,
  const Nil_cpu_task_queue queue,
  const Nil_cpu_task_fn task_fn,
  void *data)
{
  if(task_fn)
  {
    Nil_cpu_task *task_pool  = nullptr;
    size_t       *task_count = nullptr;
  
    switch(queue)
    {
      case(NIL_CPU_TASK_EARLY_THINK):
        task_pool = ctx->early_think_tasks;
        task_count = &ctx->early_think_task_count;
        break;
        
      case(NIL_CPU_TASK_THINK):
        task_pool = ctx->think_tasks;
        task_count = &ctx->think_task_count;
        break;
        
      case(NIL_CPU_TASK_LATE_THINK):
        task_pool = ctx->late_think_tasks;
        task_count = &ctx->late_think_task_count;
        break;
        
      default:
        LOG_ERROR("Unknown task pool");
        LIB_ASSERT(false);
        return;
    }
    
    if(*task_count < NIL_MAX_CPU_TASK_COUNT)
    {
      const size_t index = *task_count;
      task_pool[index].callback = task_fn;
      task_pool[index].user_data = data;
      
      *task_count += 1;
    }
  }
}


void
nil_task_gpu_add(
  Nil_ctx *ctx,
  const Nil_gpu_task_queue queue,
  const Nil_gpu_task_fn task_fn,
  void *data)
{
  if(task_fn)
  {
    Nil_cpu_task *task_pool = nullptr;
    size_t       *task_count = nullptr;
  
    switch(queue)
    {
      case(NIL_GPU_TASK_PRE_RENDER):
        task_pool = ctx->early_think_tasks;
        task_count = &ctx->early_think_task_count;
        break;
        
      case(NIL_GPU_TASK_RENDER):
        task_pool = ctx->think_tasks;
        task_count = &ctx->think_task_count;
        break;
        
      case(NIL_GPU_TASK_POST_RENDER):
        task_pool = ctx->late_think_tasks;
        task_count = &ctx->late_think_task_count;
        break;
        
      default:
        LOG_ERROR("Unknown task pool");
        LIB_ASSERT(false);
        return;
    }
    
    if(*task_count < NIL_MAX_GPU_TASK_COUNT)
    {
      const size_t index = *task_count;
      task_pool[index].callback = task_fn;
      task_pool[index].user_data = data;
      
      *task_count += 1;
    }
  }
}
