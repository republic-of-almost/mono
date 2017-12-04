#ifndef JOB_QUEUE_INCLUDED_96B763DE_730A_4B8D_8E15_01B87708C937
#define JOB_QUEUE_INCLUDED_96B763DE_730A_4B8D_8E15_01B87708C937


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------- [ Job Fwd Dec ] -- */


struct optio_mutex;
struct optio_job_desc;
struct optio_counter;


/* --------------------------------------------------------- [ Job Types ] -- */


struct optio_job_batch
{
  int total_jobs;
  int pending_jobs;
  int is_blocked;
  
  struct optio_counter *counter;
};


enum
{
  optio_JOB_STATUS_PENDING,   /* a job that has not started to be processed */
  optio_JOB_STATUS_BUSY,      /* a job that is started to be processed */
};


struct optio_job_queue_ctx
{
  /* array */ int                     *job_status;
  /* array */ unsigned                *job_ids;
  /* array */ struct optio_job_desc   *jobs;
  /* array */ int                     *job_batch_ids;
  int                                 job_id_counter;
  
  /* array */ unsigned                *batch_ids;
  /* array */ struct optio_job_batch  *batches;
  int                                 batch_id_counter;
  
  /* array */ struct optio_counter    *counters;
  
  struct optio_mutex                  *mutex;
};


/* ------------------------------------------------------ [ Job Lifetime ] -- */
/*
  Life time of the queue.
*/


void
optio_job_queue_create(struct optio_job_queue_ctx *ctx, unsigned queue_hint);


void
optio_job_queue_destroy(struct optio_job_queue_ctx *ctx);


/* ---------------------------------------------------- [ Job Attributes ] -- */
/*
  Query the state of the job queue.
*/


unsigned
optio_job_queue_batch_size(struct optio_job_queue_ctx *ctx);


unsigned
optio_job_queue_desc_size(struct optio_job_queue_ctx *ctx);


int /* returns 1 if work or 0 if none */
optio_job_queue_has_work(struct optio_job_queue_ctx *ctx);


/* ------------------------------------------------------- [ Job Actions ] -- */
/*
  Get work, add and remove work into the queue.
*/


unsigned /* returns the job_id, 0 if none found */
optio_job_queue_next(
  struct optio_job_queue_ctx *ctx,
  void **out_func,
  void **out_arg
);


unsigned /* returns the batch id */
optio_job_queue_add_batch(
  struct optio_job_queue_ctx *ctx,
  struct optio_job_desc *desc,
  unsigned count
);


struct optio_counter *
optio_job_queue_batch_block(
  struct optio_job_queue_ctx *ctx,
  unsigned batch_id
);


void
optio_job_queue_batch_unblock(
  struct optio_job_queue_ctx *ctx,  /* context */
  struct optio_counter *counter     /* the counter that is unblocked */
);


unsigned /* returns the batch_id if its is complete */
optio_job_queue_clear(
  struct optio_job_queue_ctx *ctx,
  unsigned job_id
);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
