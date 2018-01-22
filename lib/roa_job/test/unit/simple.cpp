#include <roa_job/dispatcher.h>
#include <catch/catch.hpp>


/* job will print out some text */
/*#define SIMPLE_DEBUG_OUTPUT*/


/* --------------------------------------------------------- [ Test Func ] -- */


void
test_job_01(roa_dispatcher_ctx *, void *arg)
{
  int *int_arg = reinterpret_cast<int*>(arg);
  *int_arg += 1;
  
  #ifdef SIMPLE_DEBUG_OUTPUT
  printf("job output\n");
  #endif
}


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Optio Simple")
{
  roa_dispatcher_ctx *ctx{nullptr};
  roa_dispatcher_create(&ctx, NULL);
  
  REQUIRE(ctx);
  
  //SECTION("Do nothing")
  {
    /* make sure if we bail instatnly we exit cleanly */
  }
  
  SECTION("Single Job")
  {
    int arg{0};
    
    roa_job_desc desc {
      test_job_01,
      (void*)&arg
    };
    
    roa_dispatcher_add_jobs(ctx, &desc, 1);
    roa_dispatcher_run(ctx);
    
    REQUIRE(arg == 1);
  }
  
  SECTION("10 Single Jobs")
  {
    constexpr size_t count{10};
  
    int args[count]{};
    roa_job_desc descs[count]{};
    
    for(uint32_t i = 0; i < count; ++i)
    {
      descs[i].func = test_job_01;
      descs[i].arg = &args[i];
    }
    
    roa_dispatcher_add_jobs(ctx, descs, count);
    roa_dispatcher_run(ctx);
    
    for(uint32_t i = 0; i < count; ++i)
    {
      REQUIRE(args[i] == 1);
    }
  }
  
  roa_dispatcher_destroy(&ctx);
}
