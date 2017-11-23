#include <optio/dispatcher.h>
#include <catch/catch.hpp>


/* --------------------------------------------------------- [ Test Func ] -- */


void
test_job(optio_dispatcher_ctx *ctx, void *arg)
{
  int *int_arg = reinterpret_cast<int*>(arg);
  *int_arg += 1;
}


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Optio Simple")
{
  optio_dispatcher_ctx *ctx{nullptr};
  optio_dispatcher_create(&ctx);
  
  REQUIRE(ctx);
  
  SECTION("Single Job")
  {
    int arg{0};
    
    optio_job_desc desc {
      test_job,
      (void*)&arg
    };
    
    optio_dispatcher_add_jobs(ctx, &desc, 1);
    optio_dispatcher_run(ctx);
    
    REQUIRE(arg == 1);
  }
  
  SECTION("10 Single Jobs")
  {
    constexpr size_t count{10};
  
    int args[count]{};
    optio_job_desc descs[count]{};
    
    for(uint32_t i = 0; i < count; ++i)
    {
      descs[i].func = test_job;
      descs[i].arg = &args[i];
    }
    
    optio_dispatcher_add_jobs(ctx, descs, count);
    optio_dispatcher_run(ctx);
    
    for(uint32_t i = 0; i < count; ++i)
    {
      REQUIRE(args[i] == 1);
    }
  }
  
  optio_dispatcher_destroy(&ctx);
}
