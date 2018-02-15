#include <catch/catch.hpp>
#include <roa_job/dispatcher.h>
#include <fiber_pool.hpp>
#include <counter.hpp>


void
fiber_func(void *)
{
  /* we aren't testing fibers so this should do nothing */
}


TEST_CASE("Fiber pool")
{
  roa_fiber_pool_ctx ctx;
  
  constexpr unsigned fiber_count = 10;
  roa_fiber_pool_create(&ctx, (void*)fiber_func, nullptr, fiber_count);
  
  SECTION("initalized")
  {
    REQUIRE(roa_fiber_pool_has_work(&ctx) == 0);
    REQUIRE(roa_fiber_pool_size(&ctx) == fiber_count);
    REQUIRE(roa_fiber_pool_in_flight_size(&ctx) == 0);
    REQUIRE(roa_fiber_pool_blocked_size(&ctx) == 0);
  }
  
  SECTION("in flight")
  {
    roa_fiber *fiber = roa_fiber_pool_next_free(&ctx);
    
    REQUIRE(fiber != nullptr);
    REQUIRE(roa_fiber_pool_has_work(&ctx) == 1);
    REQUIRE(roa_fiber_pool_size(&ctx) == fiber_count);
    REQUIRE(roa_fiber_pool_in_flight_size(&ctx) == 1);
    REQUIRE(roa_fiber_pool_blocked_size(&ctx) == 0);
  }
  
  SECTION("block/unblock fiber")
  {
    roa_fiber *fiber = roa_fiber_pool_next_free(&ctx);
    
    REQUIRE(fiber != nullptr);
    REQUIRE(roa_fiber_pool_has_work(&ctx) == 1);
    REQUIRE(roa_fiber_pool_size(&ctx) == fiber_count);
    REQUIRE(roa_fiber_pool_in_flight_size(&ctx) == 1);
    REQUIRE(roa_fiber_pool_blocked_size(&ctx) == 0);
    
    roa_counter counter;
    roa_counter_set(&counter, 1, 0);
    
    roa_fiber_pool_block(&ctx, fiber, &counter);
    
    REQUIRE(fiber != nullptr);
    REQUIRE(roa_fiber_pool_has_work(&ctx) == 1);
    REQUIRE(roa_fiber_pool_size(&ctx) == fiber_count);
    REQUIRE(roa_fiber_pool_in_flight_size(&ctx) == 1);
    REQUIRE(roa_fiber_pool_blocked_size(&ctx) == 1);
    
    roa_fiber *should_be_null = roa_fiber_pool_next_pending(&ctx, 0);
    REQUIRE(should_be_null == nullptr);
    
    roa_counter_decrement(&counter);
    roa_fiber *should_exist = roa_fiber_pool_next_pending(&ctx, 0);
    REQUIRE(should_exist != nullptr);
  }
  
  SECTION("Request all fibers")
  {
    roa_fiber *fibers[fiber_count]{};
    
    /* get all the fibers */
  
    for(unsigned i = 0; i < fiber_count; ++i)
    {
      fibers[i] = roa_fiber_pool_next_free(&ctx);
      REQUIRE(fibers[i] != nullptr);
      
      REQUIRE(roa_fiber_pool_has_work(&ctx) == 1);
      REQUIRE(roa_fiber_pool_size(&ctx) == fiber_count);
      REQUIRE(roa_fiber_pool_in_flight_size(&ctx) == i + 1);
      REQUIRE(roa_fiber_pool_blocked_size(&ctx) == 0);
    }

    /* this fails which is good */
//    /* one more */
//    roa_fiber *fiber = roa_fiber_pool_next_free(&ctx);
//    REQUIRE(fiber != nullptr);
  }
  
  roa_fiber_pool_destroy(&ctx);
}
