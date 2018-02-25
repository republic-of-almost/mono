#ifndef APP_DATA_INCLUDED_0423BECD_D143_4F43_AB2A_44D12C905E84
#define APP_DATA_INCLUDED_0423BECD_D143_4F43_AB2A_44D12C905E84


#include <roa_lib/atomic.h>


typedef struct roa_ctx * roa_ctx_t;
typedef struct roa_dispatcher_ctx * roa_dispatcher_ctx_t;


struct shorts_app_data
{
  /* system ctx */
  roa_ctx_t device_ctx;
  roa_dispatcher_ctx_t dispatcher_ctx;

  /* application data */
  roa_atomic_int job_spin_lock;
  struct roa_job_desc *jobs_to_tick;
};


#endif /* inc guard */