#include <roa_job/roa_job.h>

#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>


void
some_work(roa_job_dispatcher_ctx_t ctx, void *arg)
{
  /* do nothing */
}


void
some_other_work(roa_job_dispatcher_ctx_t ctx, void *arg)
{
  /* do nothing */
}

void
double_submit_crash(roa_job_dispatcher_ctx_t ctx, void *arg)
{
  roa_job_desc desc{};
  desc.func = some_work;

  roa_job_submit(ctx, &desc, 1);

  roa_job_desc other_desc{};
  desc.func = some_other_work;

  roa_job_submit(ctx, &other_desc, 1);
}


void
shutdown_failure(roa_job_dispatcher_ctx_t ctx, void *arg)
{
  roa_job_desc desc{};
  desc.func = shutdown_failure;

  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
  roa_job_submit(ctx, &desc, 1);
}


TEST_CASE("Bug")
{
  roa_job_dispatcher_ctx_t ctx{};
  roa_job_dispatcher_ctx_create(&ctx, ROA_NULL);

  SECTION("double submit crash")
  {
    roa_job_desc desc{};
    desc.func = double_submit_crash;

    roa_job_submit(ctx, &desc, 1);
    roa_job_dispatcher_ctx_run(ctx);

    REQUIRE(true);
  }

  SECTION("shutdown failure")
  {
    roa_job_desc desc{};
    desc.func = double_submit_crash;

    roa_job_submit(ctx, &desc, 1);
    roa_job_dispatcher_ctx_run(ctx);

    REQUIRE(true);
  }

  roa_job_dispatcher_ctx_destroy(&ctx);
}