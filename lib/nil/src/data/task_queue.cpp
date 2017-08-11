#include <data/task_queue.hpp>


namespace Nil {
namespace Data {


Task_queues&
get_task_queues()
{
  static Task_queues tasks;
  return tasks;
}


} // ns
} // ns
