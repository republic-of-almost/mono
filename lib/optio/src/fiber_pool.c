#include <fiber_pool.h>
#include <fiber.h>
#include <mutex.h>
#include <roa_array.h>
#include <config.h>


/* ----------------------------------------------- [ Fiber Pool Lifetime ] -- */


void
optio_fiber_pool_create(
  struct optio_fiber_pool_ctx *ctx,
  void *func,
  void *arg)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(func);
  
  FIBER_MEMZERO(ctx, sizeof(struct optio_fiber_pool_ctx));

  optio_mutex_create(&ctx->mutex);
  optio_mutex_lock(ctx->mutex);

  optio_array_create(ctx->fibers,         FIBER_MAX_FIBER_COUNT);
  optio_array_create(ctx->free_fibers,    FIBER_MAX_FIBER_COUNT);
  optio_array_create(ctx->blocked_fibers, FIBER_MAX_FIBER_COUNT);
  optio_array_create(ctx->pending_fibers, FIBER_MAX_FIBER_COUNT);
  
  for(int i = 0; i < FIBER_MAX_FIBER_COUNT; ++i)
  {
    struct optio_fiber *new_fiber = 0;
    optio_fiber_create(&new_fiber, func, arg);
    
    FIBER_ASSERT(new_fiber);
    
    optio_array_push(ctx->fibers, new_fiber);
    optio_array_push(ctx->free_fibers, new_fiber);
  }
  
  optio_mutex_unlock(ctx->mutex);
}


void
optio_fiber_pool_destroy(struct optio_fiber_pool_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);
  
  optio_mutex_lock(ctx->mutex);
  
  for(int i = 0; i < FIBER_MAX_FIBER_COUNT; ++i)
  {
    optio_fiber_destroy(ctx->fibers[i]);
  }
  
  optio_array_destroy(ctx->fibers);
  optio_array_destroy(ctx->free_fibers);
  optio_array_destroy(ctx->blocked_fibers);
  optio_array_destroy(ctx->pending_fibers);
  
  optio_mutex_unlock(ctx->mutex);
  
  optio_mutex_destroy(&ctx->mutex);
}


/* --------------------------------------------- [ Fiber Pool Attributes ] -- */


int
optio_fiber_pool_has_work(struct optio_fiber_pool_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);
  
  optio_mutex_lock(ctx->mutex);
  
  const size_t total_count = optio_array_size(ctx->fibers);
  const size_t free_count  = optio_array_size(ctx->free_fibers);
  
  optio_mutex_unlock(ctx->mutex);
  
  return total_count > free_count ? 1 : 0;
}


/* ------------------------------------------------ [ Fiber Pool Actions ] -- */


struct optio_fiber*
optio_fiber_pool_next_free(struct optio_fiber_pool_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);
  
  optio_mutex_lock(ctx->mutex);

  struct optio_fiber **free = ctx->free_fibers;
  struct optio_fiber *next = 0;
  
  const size_t count = optio_array_size(free);
  
  if(count)
  {
    next = free[count - 1];
    
    optio_array_pop(free);
  }
  else
  {
    /* todo - should we create fibers on the fly? */
    FIBER_ASSERT(0);
  }
  
  optio_mutex_unlock(ctx->mutex);
  
  return next;
}


struct optio_fiber*
optio_fiber_pool_next_pending(struct optio_fiber_pool_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);

  optio_mutex_lock(ctx->mutex);

  struct optio_fiber **pen_arr = ctx->pending_fibers;
  struct optio_fiber *next = 0;
  
  const size_t count = optio_array_size(pen_arr);
  
  if(count)
  {
    next = pen_arr[count - 1];
    unsigned job_id = optio_fiber_get_user_id(next);
    
    FIBER_ASSERT(job_id);
    
    optio_array_pop(pen_arr);
  }
  
  optio_mutex_unlock(ctx->mutex);
  
  return next;
}


void
optio_fiber_pool_block(
  struct optio_fiber_pool_ctx *ctx,
  struct optio_fiber *fiber)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(fiber);

  optio_mutex_lock(ctx->mutex);
  
  optio_array_push(ctx->blocked_fibers, fiber);
  
  optio_mutex_unlock(ctx->mutex);
}


void
optio_fiber_pool_done(
  struct optio_fiber_pool_ctx *ctx,
  struct optio_fiber *fiber)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(fiber);

  optio_mutex_lock(ctx->mutex);
  
  optio_fiber_set_user_id(fiber, 0);
  optio_fiber_set_block_id(fiber, 0);
  
  optio_array_push(ctx->free_fibers, fiber);
  
  optio_mutex_unlock(ctx->mutex);
}


void
optio_fiber_pool_unblock(
  struct optio_fiber_pool_ctx *ctx,
  int block_id)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(block_id);

  optio_mutex_lock(ctx->mutex);
  
  struct optio_fiber **blocked = ctx->blocked_fibers;
  
  const size_t count = optio_array_size(blocked);
  
  for(int i = 0; i < count; ++i)
  {
    const unsigned fi_block_id = optio_fiber_get_block_id(blocked[i]);
  
    if(fi_block_id == block_id)
    {      
      optio_array_push(ctx->pending_fibers, blocked[i]);
      optio_array_erase(blocked, i);
      
      /* todo - in theory there can be only one fiber blocked by a batch,
         we should maybe check the entire array anyway */
      break;
    }
  }
  
  optio_mutex_unlock(ctx->mutex);
}
