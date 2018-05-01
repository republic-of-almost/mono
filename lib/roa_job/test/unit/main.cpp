#include <roa_job/roa_job.h>

#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>


void
some_dummy_work(roa_job_dispatcher_ctx_t, void *) { /* do nothing */ }


void
some_other_dummy_work(roa_job_dispatcher_ctx_t, void *) { /* do nothing */ }


void
double_submit_crash(roa_job_dispatcher_ctx_t ctx, void *)
{
  roa_job_desc desc{};
  desc.func = some_dummy_work;

  roa_job_submit(ctx, &desc, 1);

  roa_job_desc other_desc{};
  other_desc.func = some_other_dummy_work;

  roa_job_submit(ctx, &other_desc, 1);
}


void
shutdown_failure(roa_job_dispatcher_ctx_t ctx, void *)
{
  roa_job_desc desc{};
  desc.func = some_dummy_work;

  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
}


TEST_CASE("Regression Tests")
{
  roa_job_dispatcher_ctx_t ctx{};
  roa_job_dispatcher_ctx_create(&ctx, ROA_NULL);

  SECTION("double submit crash")
  {
    /*
      Bug: crash (note: if submit same function twice it doesn't crash)
    */

    roa_job_desc desc{};
    desc.func = double_submit_crash;

    roa_job_submit(ctx, &desc, 1);
    roa_job_dispatcher_ctx_run(ctx);

    REQUIRE(true);
  }

  SECTION("shutdown failure")
  {
    /*
      Bug: dispatcher shutdown doesn't happen.
    */

    roa_job_desc desc{};
    desc.func = shutdown_failure;

    roa_job_submit(ctx, &desc, 1);
    roa_job_dispatcher_ctx_run(ctx);

    REQUIRE(true);
  }

  roa_job_dispatcher_ctx_destroy(&ctx);
}
