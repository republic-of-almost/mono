#include <catch/catch.hpp>
#include <roa_job/dispatcher.h>
#include <chrono>
#include <thread>


/* ------------------------------------------------------- [ Test Helper ] -- */


#define ARR_COUNT(arr) sizeof(arr) / sizeof(arr[0])


/* --------------------------------------------------------- [ Test Func ] -- */


void
inner_addition_call(roa_dispatcher_ctx *, void *arg)
{
  int *int_arg = reinterpret_cast<int*>(arg);
  
  *int_arg += 1;
}


void
outter_multiplication_call_01(roa_dispatcher_ctx *ctx, void *arg)
{
  int *int_arg = reinterpret_cast<int*>(arg);
  
  roa_job_desc desc
  {
    inner_addition_call,
    arg
  };
  
  
  unsigned marker = roa_dispatcher_add_jobs(ctx, &desc, 1);
  roa_dispatcher_wait_for_counter(ctx, marker);
  
  *int_arg *= 0;
}

void
outter_multiplication_call_04(roa_dispatcher_ctx *ctx, void *arg)
{
  int *int_arg = reinterpret_cast<int*>(arg);
  
  roa_job_desc desc[]
  {
    {inner_addition_call, arg},
    {inner_addition_call, arg},
    {inner_addition_call, arg},
    {inner_addition_call, arg},
  };
  
  unsigned marker = roa_dispatcher_add_jobs(ctx, desc, ARR_COUNT(desc));
  roa_dispatcher_wait_for_counter(ctx, marker);
  
  *int_arg *= 0;
}


void
inner_wait_call_01(roa_dispatcher_ctx *, void *)
{
}


void
outter_wait_call_01(roa_dispatcher_ctx *ctx, void *arg)
{
  int *int_arg = reinterpret_cast<int*>(arg);
  
  roa_job_desc desc[]
  {
    {inner_wait_call_01, arg},
  };
  
  unsigned marker = roa_dispatcher_add_jobs(ctx, desc, ARR_COUNT(desc));
  
  /* sleep this job should be done */
  std::this_thread::sleep_for(
    std::chrono::milliseconds(rand() % 4)
  );
  
  roa_dispatcher_wait_for_counter(ctx, marker);
  
  *int_arg *= 0;
}


/* --------------------------------------------------------- [ Test Case ] -- */


TEST_CASE("Optio Nested")
{
  roa_dispatcher_ctx *ctx{nullptr};

  const roa_dispatcher_desc ctx_desc{
    -1,   /* free_cores */
  };
  roa_dispatcher_create(&ctx, &ctx_desc);
  
  REQUIRE(ctx);
  
  SECTION("Single Nested")
  {
    /*
      Two tasks are spawned,
      first should block and spawn a second.
      second should add by one.
      first should resume and mul by 0.
    */
    int arg{0};
    
    roa_job_desc desc {
      outter_multiplication_call_01,
      (void*)&arg
    };
    
    roa_dispatcher_add_jobs(ctx, &desc, 1);
    roa_dispatcher_run(ctx);
    
    REQUIRE(arg == 0);
  }
  
  SECTION("Multi Nested")
  {
    /*
      Two tasks are spawned,
      first should block and spawn a number of others.
      others should add by one.
      first should resume and mul by 0.
    */
    int args[10]{};
    
    roa_job_desc descs[ARR_COUNT(args)];
    
    for(uint32_t i = 0; i < ARR_COUNT(descs); ++i)
    {
      descs[i].func = outter_multiplication_call_04;
      descs[i].arg = &args[i];
    }
    
    roa_dispatcher_add_jobs(ctx, descs, ARR_COUNT(descs));
    roa_dispatcher_run(ctx);
    
    for(uint32_t i = 0; i < ARR_COUNT(descs); ++i)
    {
      REQUIRE(args[i] == 0);
    }
  }
  
  SECTION("Wait On Completed Job")
  {
    /*
     
    */
    int arg{0};
    
    roa_job_desc desc {
      outter_wait_call_01,
      (void*)&arg
    };
    
    roa_dispatcher_add_jobs(ctx, &desc, 1);
    roa_dispatcher_run(ctx);
    
    REQUIRE(arg == 0);
  }
  
  roa_dispatcher_destroy(&ctx);
}
