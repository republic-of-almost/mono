#ifndef ROA_JOB_INCLUDED_F14B5BF7_A2A2_4A6F_B5DD_E66FB239C8EA
#define ROA_JOB_INCLUDED_F14B5BF7_A2A2_4A6F_B5DD_E66FB239C8EA


#include <roa_lib/fundamental.h>


/* ------------------------------------------------------------- [ types ] -- */


typedef struct roa_job_dispatcher_ctx * roa_job_dispatcher_ctx_t;


/* ---------------------------------------------------------- [ lifetime ] -- */


struct roa_job_dispatcher_setup_desc
{
  int max_cores;
  int spare_cores;
};


void
roa_job_dispatcher_ctx_create(
  roa_job_dispatcher_ctx_t *ctx,
  struct roa_job_dispatcher_setup_desc *desc);


void
roa_job_dispatcher_ctx_run(
  roa_job_dispatcher_ctx_t ctx);


void
roa_job_dispatcher_ctx_destroy(
  roa_job_dispatcher_ctx_t *ctx);


struct roa_job_dispatcher_desc
{
  unsigned sizeof_thread_pool;
};


void
roa_job_dispatcher_ctx_get_desc(
  roa_job_dispatcher_ctx_t ctx,
  struct roa_job_dispatcher_desc *desc);


/* -------------------------------------------------------------- [ Jobs ] -- */


typedef void(*roa_job_fn)(roa_job_dispatcher_ctx_t ctx, void *arg);


struct roa_job_desc
{
  roa_job_fn  job_function;
  void*       job_arg;
  ROA_BOOL    thread_locked;
};


uint64_t
roa_job_submit(
  roa_job_dispatcher_ctx_t ctx,
  struct roa_job_desc *desc,
  unsigned count);


void
roa_job_wait(
  roa_job_dispatcher_ctx_t ctx,
  uint64_t job_marker);


#endif /* inc guard */