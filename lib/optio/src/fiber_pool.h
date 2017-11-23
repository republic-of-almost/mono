#ifndef FIBER_POOL_INCLUDED_3A6C2528_FB96_4C14_85EA_C1D60DF71074
#define FIBER_POOL_INCLUDED_3A6C2528_FB96_4C14_85EA_C1D60DF71074


/* ------------------------------------------------ [ Fiber Pool Fwd Dec ] -- */


struct optio_fiber;
struct optio_mutex;


/* -------------------------------------------------- [ Fiber Pool Types ] -- */


struct optio_fiber_pool_ctx
{
  /* array */ struct optio_fiber       **fibers;
  /* array */ struct optio_fiber       **free_fibers;
  /* array */ struct optio_fiber       **blocked_fibers;
  /* array */ struct optio_fiber       **pending_fibers;
  
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
  void *arg);


void
optio_fiber_pool_destroy(struct optio_fiber_pool_ctx *ctx);


/* --------------------------------------------- [ Fiber Pool Attributes ] -- */
/*
  Check the state of the pool
*/


int /* returns 1 if work, 0 if none */
optio_fiber_pool_has_work(struct optio_fiber_pool_ctx *ctx);


/* ------------------------------------------------ [ Fiber Pool Actions ] -- */
/*
  Get and block fibers
*/


struct optio_fiber*
optio_fiber_pool_next_free(struct optio_fiber_pool_ctx *ctx);


struct optio_fiber*
optio_fiber_pool_next_pending(struct optio_fiber_pool_ctx *ctx);


void
optio_fiber_pool_block(struct optio_fiber_pool_ctx *ctx, struct optio_fiber *fiber);


void
optio_fiber_pool_unblock(struct optio_fiber_pool_ctx *ctx, int block_id);


void
optio_fiber_pool_done(struct optio_fiber_pool_ctx *ctx, struct optio_fiber *fiber);


#endif /* inc guard */
