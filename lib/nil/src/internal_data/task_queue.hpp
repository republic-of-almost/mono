#ifndef TASK_QUEUE_INCLUDED_2BA00441_1AD8_42FE_80D9_6A5E3538451C
#define TASK_QUEUE_INCLUDED_2BA00441_1AD8_42FE_80D9_6A5E3538451C


#include <nil/fwd.hpp>
#include <internal_data/internal_data.hpp>


void
nil_task_cpu_process(
  Nil_ctx *ctx,
  const Nil_cpu_task *tasks,
  const size_t count);


void
nil_task_gpu_process(
  Nil_ctx *ctx,
  const Nil_gpu_task *tasks,
  const size_t count);


#endif // inc guard
