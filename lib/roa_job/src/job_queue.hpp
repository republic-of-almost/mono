#ifndef JOB_QUEUE_INCLUDED_96B763DE_730A_4B8D_8E15_01B87708C937
#define JOB_QUEUE_INCLUDED_96B763DE_730A_4B8D_8E15_01B87708C937


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Job Fwd Dec ] -- */


typedef void* roa_mutex;
struct roa_job_desc;
struct roa_counter;


/* --------------------------------------------------------- [ Job Types ] -- */


struct roa_job_batch
{
  int total_jobs;
  int pending_jobs;
  int is_blocked;
  
  struct roa_counter *counter;
};


enum
{
  roa_JOB_STATUS_PENDING,   /* a job that has not started to be processed */
  roa_JOB_STATUS_BUSY,      /* a job that is started to be processed */
};


struct roa_job_queue_ctx
{
  /* array */ int                     *job_status;
  /* array */ unsigned                *job_ids;
  /* array */ struct roa_job_desc   *jobs;
  /* array */ int                     *job_batch_ids;
  int                                 job_id_counter;
  
  /* array */ unsigned                *batch_ids;
  /* array */ struct roa_job_batch  *batches;
  int                                 batch_id_counter;
  
  /* array */ struct roa_counter    *counters;
  
  roa_mutex                            mutex;
};


/* ------------------------------------------------------ [ Job Lifetime ] -- */
/*
  Life time of the queue.
*/


void
roa_job_queue_create(struct roa_job_queue_ctx *ctx, unsigned queue_hint);


void
roa_job_queue_destroy(struct roa_job_queue_ctx *ctx);


/* ---------------------------------------------------- [ Job Attributes ] -- */
/*
  Query the state of the job queue.
*/


unsigned
roa_job_queue_batch_size(struct roa_job_queue_ctx *ctx);


unsigned
roa_job_queue_desc_size(struct roa_job_queue_ctx *ctx);


int /* returns 1 if work or 0 if none */
roa_job_queue_has_work(struct roa_job_queue_ctx *ctx);


/* ------------------------------------------------------- [ Job Actions ] -- */
/*
  Get work, add and remove work into the queue.
*/


unsigned /* returns the job_id, 0 if none found */
roa_job_queue_next(
  struct roa_job_queue_ctx *ctx,
  int th_id,
  void **out_func,
  void **out_arg
);


unsigned /* returns the batch id */
roa_job_queue_add_batch(
  struct roa_job_queue_ctx *ctx,
  struct roa_job_desc *desc,
  unsigned count,
  int th_id
);


struct roa_counter *
roa_job_queue_batch_block(
  struct roa_job_queue_ctx *ctx,
  unsigned batch_id
);


void
roa_job_queue_batch_unblock(
  struct roa_job_queue_ctx *ctx,  /* context */
  struct roa_counter *counter     /* the counter that is unblocked */
);


unsigned /* returns the batch_id if its is complete */
roa_job_queue_clear(
  struct roa_job_queue_ctx *ctx,
  unsigned job_id
);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
