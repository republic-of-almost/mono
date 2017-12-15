#include <catch/catch.hpp>
#include <optio/dispatcher.h>
#include <fiber_pool.hpp>
#include <counter.hpp>


void
fiber_func(void *)
{
  /* we aren't testing fibers so this should do nothing */
}


TEST_CASE("Fiber pool")
{
  optio_fiber_pool_ctx ctx;
  
  constexpr unsigned fiber_count = 10;
  optio_fiber_pool_create(&ctx, (void*)fiber_func, nullptr, fiber_count);
  
  SECTION("initalized")
  {
    REQUIRE(optio_fiber_pool_has_work(&ctx) == 0);
    REQUIRE(optio_fiber_pool_size(&ctx) == fiber_count);
    REQUIRE(optio_fiber_pool_in_flight_size(&ctx) == 0);
    REQUIRE(optio_fiber_pool_blocked_size(&ctx) == 0);
  }
  
  SECTION("in flight")
  {
    optio_fiber *fiber = optio_fiber_pool_next_free(&ctx);
    
    REQUIRE(fiber != nullptr);
    REQUIRE(optio_fiber_pool_has_work(&ctx) == 1);
    REQUIRE(optio_fiber_pool_size(&ctx) == fiber_count);
    REQUIRE(optio_fiber_pool_in_flight_size(&ctx) == 1);
    REQUIRE(optio_fiber_pool_blocked_size(&ctx) == 0);
  }
  
  SECTION("block/unblock fiber")
  {
    optio_fiber *fiber = optio_fiber_pool_next_free(&ctx);
    
    REQUIRE(fiber != nullptr);
    REQUIRE(optio_fiber_pool_has_work(&ctx) == 1);
    REQUIRE(optio_fiber_pool_size(&ctx) == fiber_count);
    REQUIRE(optio_fiber_pool_in_flight_size(&ctx) == 1);
    REQUIRE(optio_fiber_pool_blocked_size(&ctx) == 0);
    
    optio_counter counter;
    optio_counter_set(&counter, 1, 0);
    
    optio_fiber_pool_block(&ctx, fiber, &counter);
    
    REQUIRE(fiber != nullptr);
    REQUIRE(optio_fiber_pool_has_work(&ctx) == 1);
    REQUIRE(optio_fiber_pool_size(&ctx) == fiber_count);
    REQUIRE(optio_fiber_pool_in_flight_size(&ctx) == 1);
    REQUIRE(optio_fiber_pool_blocked_size(&ctx) == 1);
    
    optio_fiber *should_be_null = optio_fiber_pool_next_pending(&ctx);
    REQUIRE(should_be_null == nullptr);
    
    optio_counter_decrement(&counter);
    optio_fiber *should_exist = optio_fiber_pool_next_pending(&ctx);
    REQUIRE(should_exist != nullptr);
  }
  
  SECTION("Request all fibers")
  {
    optio_fiber *fibers[fiber_count]{};
    
    /* get all the fibers */
  
    for(unsigned i = 0; i < fiber_count; ++i)
    {
      fibers[i] = optio_fiber_pool_next_free(&ctx);
      REQUIRE(fibers[i] != nullptr);
      
      REQUIRE(optio_fiber_pool_has_work(&ctx) == 1);
      REQUIRE(optio_fiber_pool_size(&ctx) == fiber_count);
      REQUIRE(optio_fiber_pool_in_flight_size(&ctx) == i + 1);
      REQUIRE(optio_fiber_pool_blocked_size(&ctx) == 0);
    }

    /* this fails which is good */
//    /* one more */
//    optio_fiber *fiber = optio_fiber_pool_next_free(&ctx);
//    REQUIRE(fiber != nullptr);
  }
  
  optio_fiber_pool_destroy(&ctx);
}
