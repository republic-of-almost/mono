#ifndef CTX_INCLUDED_DC185702_6D52_41BD_89A1_47D12D223CA0
#define CTX_INCLUDED_DC185702_6D52_41BD_89A1_47D12D223CA0


#include <roa_lib/signal.h>


struct thread_local_storage;

typedef void* roa_thread;
typedef void* roa_thread_id;


typedef enum _dispatch_status
{
  DISPATCH_STARTUP,
  DISPATCH_WORKING,
  DISPATCH_SHUTDOWN,
} dispatch_status;


struct roa_job_dispatcher_ctx
{
  dispatch_status status;

  /* array */ struct thread_local_storage *tls;
  /* array */ roa_thread_id *thread_ids;
  /* array */ roa_thread *threads;

  /* signals */
  roa_signal signal_start;
  roa_signal signal_new_work;

  unsigned thread_count;
};


#endif /* inc guard */