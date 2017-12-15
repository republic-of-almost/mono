#include <fiber_pool.hpp>
#include <fiber.hpp>
#include <mutex.hpp>
#include <roa_array.hpp>
#include <config.hpp>
#include <counter.hpp>


/* ----------------------------------------------- [ Fiber Pool Lifetime ] -- */


void
optio_fiber_pool_create(
  struct optio_fiber_pool_ctx *ctx,
  void *func,
  void *arg,
  unsigned fiber_count)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(func);
  
  FIBER_MEMZERO(ctx, sizeof(struct optio_fiber_pool_ctx));

  optio_mutex_create(&ctx->mutex);
  optio_mutex_lock(ctx->mutex);
  
  const unsigned count = fiber_count;

  optio_array_create(ctx->fibers,           count);
  optio_array_create(ctx->free_fibers,      count);
  optio_array_create(ctx->blocked_fibers,   count);
  optio_array_create(ctx->blocked_counters, count);
  
  for(unsigned i = 0; i < count; ++i)
  {
    struct optio_fiber *new_fiber = 0;
    optio_fiber_create(&new_fiber, (optio_fiber_func)func, arg);
    
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
  
  size_t count = optio_array_size(ctx->fibers);
  
  for(size_t i = 0; i < count; ++i)
  {
    optio_fiber_destroy(ctx->fibers[i]);
  }
  
  optio_array_destroy(ctx->fibers);
  optio_array_destroy(ctx->free_fibers);
  optio_array_destroy(ctx->blocked_fibers);
  optio_array_destroy(ctx->blocked_counters);
  
  optio_mutex_unlock(ctx->mutex);
  
  optio_mutex_destroy(&ctx->mutex);
}


/* --------------------------------------------- [ Fiber Pool Attributes ] -- */


unsigned
optio_fiber_pool_size(struct optio_fiber_pool_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);
  
  optio_mutex_lock(ctx->mutex);
  
  const size_t count = optio_array_size(ctx->fibers);
  
  optio_mutex_unlock(ctx->mutex);
  
  return (unsigned)count;
}


unsigned
optio_fiber_pool_in_flight_size(struct optio_fiber_pool_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);
  
  optio_mutex_lock(ctx->mutex);
  
  const size_t total_fibers = optio_array_size(ctx->fibers);
  const size_t free_fibers = optio_array_size(ctx->free_fibers);
  
  optio_mutex_unlock(ctx->mutex);
  
  return (unsigned)(total_fibers - free_fibers);
}


unsigned
optio_fiber_pool_blocked_size(struct optio_fiber_pool_ctx *ctx)
{
  /* param check */
  FIBER_ASSERT(ctx);

  optio_mutex_lock(ctx->mutex);
  
  const size_t blocked = optio_array_size(ctx->blocked_fibers);
  
  optio_mutex_unlock(ctx->mutex);
  
  return (unsigned)blocked;
}


unsigned
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
    next = optio_array_back(free);
    
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

  struct optio_fiber *next = NULL;

  /* check blocked fibers for ones that are now ready */
  {
    const size_t count = optio_array_size(ctx->blocked_counters);
    
    for(size_t i = 0; i < count; ++i)
    {
      const int counter = optio_counter_value(ctx->blocked_counters[i]);
      
      if(counter <= 0)
      {
        next = ctx->blocked_fibers[i];
        
        optio_array_erase(ctx->blocked_fibers, i);
        optio_array_erase(ctx->blocked_counters, i);
        
        break;
      }
    }
  }
  
  optio_mutex_unlock(ctx->mutex);
  
  return next;
}


void
optio_fiber_pool_block(
  struct optio_fiber_pool_ctx *ctx,
  struct optio_fiber *fiber,
  struct optio_counter *counter)
{
  /* param check */
  FIBER_ASSERT(ctx);
  FIBER_ASSERT(fiber);
  FIBER_ASSERT(counter);

  optio_mutex_lock(ctx->mutex);
  
  optio_array_push(ctx->blocked_counters, counter);
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
  
  optio_array_push(ctx->free_fibers, fiber);
  
  optio_mutex_unlock(ctx->mutex);
}

