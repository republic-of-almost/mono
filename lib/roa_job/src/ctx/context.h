#ifndef CTX_INCLUDED_DC185702_6D52_41BD_89A1_47D12D223CA0
#define CTX_INCLUDED_DC185702_6D52_41BD_89A1_47D12D223CA0


struct thread_local_storage;

typedef void* roa_thread;
typedef void* roa_thread_id;


struct roa_job_dispatcher_ctx
{
  /* array */ struct thread_local_storage *tls;
  /* array */ roa_thread_id *thread_ids;
  /* array */ roa_thread *threads;

  unsigned thread_count;
};


#endif /* inc guard */