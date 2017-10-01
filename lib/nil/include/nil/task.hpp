#ifndef TASK_INCLUDED_A1087C62_BA4D_4992_8A72_E3E163AB8317
#define TASK_INCLUDED_A1087C62_BA4D_4992_8A72_E3E163AB8317


#include <nil/fwd.hpp>


void
nil_task_cpu_add(
  Nil_ctx *ctx,
  const Nil_cpu_task_queue queue,
  const Nil_cpu_task_fn task_fn,
  void *data);


void
nil_task_gpu_add(
  Nil_ctx *ctx,
  const Nil_gpu_task_queue queue,
  const Nil_gpu_task_fn task_fn,
  void *data);


#endif // inc guard
