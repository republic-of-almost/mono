#ifndef FIBER_POOL_INCLUDED_3A6C2528_FB96_4C14_85EA_C1D60DF71074
#define FIBER_POOL_INCLUDED_3A6C2528_FB96_4C14_85EA_C1D60DF71074


/* ------------------------------------------------ [ Fiber Pool Fwd Dec ] -- */


struct optio_fiber;
struct optio_mutex;
struct optio_counter;


/* -------------------------------------------------- [ Fiber Pool Types ] -- */


struct optio_fiber_pool_ctx
{
  /* array */ struct optio_fiber       **fibers;
  /* array */ struct optio_fiber       **free_fibers;
  /* array */ struct optio_fiber       **blocked_fibers;
  /* array */ struct optio_counter     **blocked_counters;
  
  struct optio_mutex                   *mutex;
};


/* ----------------------------------------------- [ Fiber Pool Lifetime ] -- */
/*
  Create and destroy fiber pool.
*/


void
optio_fiber_pool_create(
  struct optio_fiber_pool_ctx *ctx,
  void *func,
  void *arg,
  unsigned fiber_count /* 0 for default */
);


void
optio_fiber_pool_destroy(struct optio_fiber_pool_ctx *ctx);


/* --------------------------------------------- [ Fiber Pool Attributes ] -- */
/*
  Check the state of the pool
*/


unsigned
optio_fiber_pool_size(struct optio_fiber_pool_ctx *ctx);


unsigned
optio_fiber_pool_in_flight_size(struct optio_fiber_pool_ctx *ctx);


unsigned
optio_fiber_pool_blocked_size(struct optio_fiber_pool_ctx *ctx);


unsigned /* returns 1 if work, 0 if none */
optio_fiber_pool_has_work(struct optio_fiber_pool_ctx *ctx);


/* ------------------------------------------------ [ Fiber Pool Actions ] -- */
/*
  Fiber interaction.
  You can get the next pending or unblocked fiber, or block a fiber. When you 
  have finished with a fiber return it to the pool.
*/


struct optio_fiber*
optio_fiber_pool_next_free(struct optio_fiber_pool_ctx *ctx);


struct optio_fiber*
optio_fiber_pool_next_pending(struct optio_fiber_pool_ctx *ctx);


void
optio_fiber_pool_block(
  struct optio_fiber_pool_ctx *ctx,
  struct optio_fiber *fiber,
  struct optio_counter *counter
);


void
optio_fiber_pool_done(
  struct optio_fiber_pool_ctx *ctx,
  struct optio_fiber *fiber
);


#endif /* inc guard */
