#include <fiber_pool.hpp>
#include <fiber.hpp>
#include <roa_lib/mutex.h>
#include <roa_lib/array.h>
#include <config.hpp>
#include <counter.hpp>
#include <roa_lib/assert.h>


/* ----------------------------------------------- [ Fiber Pool Lifetime ] -- */


void
roa_fiber_pool_create(
  struct roa_fiber_pool_ctx *ctx,
  void *func,
  void *arg,
  unsigned fiber_count)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(func);
  
  memset(ctx, 0, sizeof(*ctx));

  ctx->mutex = roa_mutex_create();
  roa_mutex_lock(ctx->mutex);
  
  const unsigned count = fiber_count;

  roa_array_create(ctx->fibers,           count);
  roa_array_create(ctx->free_fibers,      count);
  roa_array_create(ctx->blocked_fibers,   count);
  roa_array_create(ctx->blocked_counters, count);
  
  for(unsigned i = 0; i < count; ++i)
  {
    struct roa_fiber *new_fiber = 0;
    roa_fiber_create(&new_fiber, (roa_fiber_func)func, arg);
    
    ROA_ASSERT(new_fiber);
    
    roa_array_push(ctx->fibers, new_fiber);
    roa_array_push(ctx->free_fibers, new_fiber);
  }
  
  roa_mutex_unlock(ctx->mutex);
}


void
roa_fiber_pool_destroy(struct roa_fiber_pool_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  
  roa_mutex_lock(ctx->mutex);
  
  size_t count = roa_array_size(ctx->fibers);
  
  for(size_t i = 0; i < count; ++i)
  {
    roa_fiber_destroy(ctx->fibers[i]);
  }
  
  roa_array_destroy(ctx->fibers);
  roa_array_destroy(ctx->free_fibers);
  roa_array_destroy(ctx->blocked_fibers);
  roa_array_destroy(ctx->blocked_counters);
  
  roa_mutex_unlock(ctx->mutex);
  
  roa_mutex_destroy(&ctx->mutex);
}


/* --------------------------------------------- [ Fiber Pool Attributes ] -- */


unsigned
roa_fiber_pool_size(struct roa_fiber_pool_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  
  roa_mutex_lock(ctx->mutex);
  
  const size_t count = roa_array_size(ctx->fibers);
  
  roa_mutex_unlock(ctx->mutex);
  
  return (unsigned)count;
}


unsigned
roa_fiber_pool_in_flight_size(struct roa_fiber_pool_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  
  roa_mutex_lock(ctx->mutex);
  
  const size_t total_fibers = roa_array_size(ctx->fibers);
  const size_t free_fibers = roa_array_size(ctx->free_fibers);
  
  roa_mutex_unlock(ctx->mutex);
  
  return (unsigned)(total_fibers - free_fibers);
}


unsigned
roa_fiber_pool_blocked_size(struct roa_fiber_pool_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);

  roa_mutex_lock(ctx->mutex);
  
  const size_t blocked = roa_array_size(ctx->blocked_fibers);
  
  roa_mutex_unlock(ctx->mutex);
  
  return (unsigned)blocked;
}


unsigned
roa_fiber_pool_has_work(struct roa_fiber_pool_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  
  roa_mutex_lock(ctx->mutex);
  
  const size_t total_count = roa_array_size(ctx->fibers);
  const size_t free_count  = roa_array_size(ctx->free_fibers);
  
  roa_mutex_unlock(ctx->mutex);
  
  return total_count > free_count ? 1 : 0;
}


/* ------------------------------------------------ [ Fiber Pool Actions ] -- */


struct roa_fiber*
roa_fiber_pool_next_free(struct roa_fiber_pool_ctx *ctx)
{
  /* param check */
  ROA_ASSERT(ctx);
  
  roa_mutex_lock(ctx->mutex);

  struct roa_fiber **free = ctx->free_fibers;
  struct roa_fiber *next = 0;
  
  const size_t count = roa_array_size(free);
  
  if(count)
  {
    next = roa_array_back(free);
    
    roa_array_pop(free);
  }
  else
  {
    /* todo - should we create fibers on the fly? */
    ROA_ASSERT(0);
  }
  
  roa_mutex_unlock(ctx->mutex);
  
  return next;
}


struct roa_fiber*
roa_fiber_pool_next_pending(
  struct roa_fiber_pool_ctx *ctx,
  int thread_id)
{
  /* param check */
  ROA_ASSERT(ctx);

  roa_mutex_lock(ctx->mutex);

  struct roa_fiber *next = NULL;

  /* check blocked fibers for ones that are now ready */
  {
    const size_t count = roa_array_size(ctx->blocked_counters);
    
    for(size_t i = 0; i < count; ++i)
    {
      struct roa_counter *counter = ctx->blocked_counters[i];

      if (counter->thread_id != -1)
      {
        if (counter->thread_id != thread_id)
        {
          continue;
        }
      }

      const int counter_value = roa_counter_value(counter);
      
      if(counter_value <= 0)
      {
        next = ctx->blocked_fibers[i];
        
        roa_array_erase(ctx->blocked_fibers, i);
        roa_array_erase(ctx->blocked_counters, i);
        
        break;
      }
    }
  }
  
  roa_mutex_unlock(ctx->mutex);
  
  return next;
}


void
roa_fiber_pool_block(
  struct roa_fiber_pool_ctx *ctx,
  struct roa_fiber *fiber,
  struct roa_counter *counter)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(fiber);
  ROA_ASSERT(counter);

  roa_mutex_lock(ctx->mutex);
  
  roa_array_push(ctx->blocked_counters, counter);
  roa_array_push(ctx->blocked_fibers, fiber);
  
  roa_mutex_unlock(ctx->mutex);
}


void
roa_fiber_pool_done(
  struct roa_fiber_pool_ctx *ctx,
  struct roa_fiber *fiber)
{
  /* param check */
  ROA_ASSERT(ctx);
  ROA_ASSERT(fiber);
  
  roa_mutex_lock(ctx->mutex);
 
  roa_array_push(ctx->free_fibers, fiber);
  
  roa_mutex_unlock(ctx->mutex);
}

