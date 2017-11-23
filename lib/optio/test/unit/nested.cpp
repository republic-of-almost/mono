#include <optio/dispatcher.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Func ] -- */


void
inner_call(optio_dispatcher_ctx *ctx, void *arg)
{
  int *int_arg = reinterpret_cast<int*>(arg);
  
  REQUIRE(*int_arg == 0);
  
  *int_arg += 1;
}


void
outter_call(optio_dispatcher_ctx *ctx, void *arg)
{
  int *int_arg = reinterpret_cast<int*>(arg);
  
  optio_job_desc desc {
    inner_call,
    arg
  };
  
  int marker = optio_dispatcher_add_jobs(ctx, &desc, 1);
  optio_dispatcher_wait_for_counter(ctx, marker);
  
  REQUIRE(*int_arg == 1);
  
  *int_arg += 1;
}


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Optio Nested")
{
  optio_dispatcher_ctx *ctx{nullptr};
  optio_dispatcher_create(&ctx);
  
  REQUIRE(ctx);
  
  SECTION("Single Blocking Job")
  {
    int arg{0};
    
    optio_job_desc desc {
      outter_call,
      (void*)&arg
    };
    
    optio_dispatcher_add_jobs(ctx, &desc, 1);
    optio_dispatcher_run(ctx);
    
    REQUIRE(arg == 2);
  }
  
//  SECTION("10 Single Jobs")
//  {
//    constexpr size_t count{10};
//  
//    int args[count]{};
//    optio_job_desc descs[count]{};
//    
//    for(uint32_t i = 0; i < count; ++i)
//    {
//      descs[i].func = test_job;
//      descs[i].arg = &args[i];
//    }
//    
//    optio_dispatcher_add_jobs(ctx, descs, count);
//    optio_dispatcher_run(ctx);
//    
//    for(uint32_t i = 0; i < count; ++i)
//    {
//      REQUIRE(args[i] == 1);
//    }
//  }
  
  optio_dispatcher_destroy(&ctx);
}
