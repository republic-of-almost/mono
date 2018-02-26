#ifndef APP_DATA_INCLUDED_0423BECD_D143_4F43_AB2A_44D12C905E84
#define APP_DATA_INCLUDED_0423BECD_D143_4F43_AB2A_44D12C905E84


#include <roa_lib/atomic.h>


typedef struct roa_ctx * roa_ctx_t;
typedef struct roa_dispatcher_ctx * roa_dispatcher_ctx_t;
typedef struct volt_ctx * volt_ctx_t;


struct shorts_app_data
{
  /* system ctx */
  roa_ctx_t device_ctx;
  roa_dispatcher_ctx_t dispatcher_ctx;
  volt_ctx_t volt_ctx;

  /* application jobs */
  roa_atomic_int think_lock;
  struct roa_job_desc *think_jobs;

  roa_atomic_int render_lock;
  struct roa_job_desc *render_jobs;
};


#endif /* inc guard */