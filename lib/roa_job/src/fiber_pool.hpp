#ifndef FIBER_POOL_INCLUDED_3A6C2528_FB96_4C14_85EA_C1D60DF71074
#define FIBER_POOL_INCLUDED_3A6C2528_FB96_4C14_85EA_C1D60DF71074


/* ------------------------------------------------ [ Fiber Pool Fwd Dec ] -- */


struct roa_fiber;
struct roa_counter;
typedef void* roa_mutex;


/* -------------------------------------------------- [ Fiber Pool Types ] -- */


struct roa_fiber_pool_ctx
{
  /* array */ struct roa_fiber        **fibers;
  /* array */ struct roa_fiber        **free_fibers;
  /* array */ struct roa_fiber        **blocked_fibers;
  /* array */ struct roa_counter      **blocked_counters;
  
  roa_mutex                           mutex;
};


/* ----------------------------------------------- [ Fiber Pool Lifetime ] -- */
/*
  Create and destroy fiber pool.
*/


void
roa_fiber_pool_create(
  struct roa_fiber_pool_ctx *ctx,
  void *func,
  void *arg,
  unsigned fiber_count /* 0 for default */
);


void
roa_fiber_pool_destroy(struct roa_fiber_pool_ctx *ctx);


/* --------------------------------------------- [ Fiber Pool Attributes ] -- */
/*
  Check the state of the pool
*/


unsigned
roa_fiber_pool_size(struct roa_fiber_pool_ctx *ctx);


unsigned
roa_fiber_pool_in_flight_size(struct roa_fiber_pool_ctx *ctx);


unsigned
roa_fiber_pool_blocked_size(struct roa_fiber_pool_ctx *ctx);


unsigned /* returns 1 if work, 0 if none */
roa_fiber_pool_has_work(struct roa_fiber_pool_ctx *ctx);


/* ------------------------------------------------ [ Fiber Pool Actions ] -- */
/*
  Fiber interaction.
  You can get the next pending or unblocked fiber, or block a fiber. When you 
  have finished with a fiber return it to the pool.
*/


struct roa_fiber*
roa_fiber_pool_next_free(struct roa_fiber_pool_ctx *ctx);


struct roa_fiber*
roa_fiber_pool_next_pending(
  struct roa_fiber_pool_ctx *ctx,
  int thread_id,
  int *is_locked);


void
roa_fiber_pool_block(
  struct roa_fiber_pool_ctx *ctx,
  struct roa_fiber *fiber,
  struct roa_counter *counter
);


void
roa_fiber_pool_done(
  struct roa_fiber_pool_ctx *ctx,
  struct roa_fiber *fiber
);


#endif /* inc guard */
