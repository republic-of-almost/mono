#include <roa_job/roa_job.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <string.h>
#include <stdio.h>


/* -------------------------------------------------------------- [ Vars ] -- */


#define TEST_WITH_OUTPUT 1


#define BATCH_COUNT (1 << 9)
#define TICK_COUNT (1 << 16)


int ticks = TICK_COUNT;
int *test_data;


/* --------------------------------------------------------------- [ Fwd ] -- */


void submit_tick(roa_job_dispatcher_ctx_t ctx);


/* --------------------------------------------------------- [ Test Jobs ] -- */


void
calculate(roa_job_dispatcher_ctx_t ctx, void *arg)
{
  if (ROA_IS_ENABLED(TEST_WITH_OUTPUT))
  {
    printf("calc job\n");
  }

  int *int_arg = (int*)arg;

  *int_arg += 1;
}


void
tick(roa_job_dispatcher_ctx_t ctx, void *arg)
{
  if (ROA_IS_ENABLED(TEST_WITH_OUTPUT))
  {
    printf("tick %d\n", TICK_COUNT - ticks);
  }

  /* submit batch of jobs */
  {
    int i;

    struct roa_job_desc *batch = ROA_NULL;
    unsigned batch_bytes = sizeof(*batch) * BATCH_COUNT;
    batch = roa_zalloc(batch_bytes);

    for (i = 0; i < BATCH_COUNT; ++i)
    {
      batch[i].func = calculate;
      batch[i].arg = (void*)&test_data[i];
      batch[i].thread_locked = 0;
    }

    uint64_t batch_id = roa_job_submit(
      ctx,
      batch,
      BATCH_COUNT
    );

    roa_job_wait(ctx, batch_id);
    roa_free(batch);
  }

  /* another tick? */
  if (ticks > 0)
  {
    submit_tick(ctx);
  }
}


/* ------------------------------------------------------- [ Helper Func ] -- */


void
submit_tick(roa_job_dispatcher_ctx_t ctx)
{
  struct roa_job_desc desc[1];
  desc[0].func = tick;
  desc[0].arg = (void*)&ticks;
  desc[0].thread_locked = ROA_FALSE;

  roa_job_submit(ctx, ROA_ARR_DATA(desc), ROA_ARR_COUNT(desc));

  ticks -= 1;
}


/* -------------------------------------------------------------- [ Main ] -- */


int
main()
{
  roa_job_dispatcher_ctx_t ctx = ROA_NULL;
  roa_job_dispatcher_ctx_create(&ctx, 0);

  test_data = roa_zalloc(sizeof(test_data[0]) * BATCH_COUNT);

  submit_tick(ctx);
  roa_job_dispatcher_ctx_run(ctx);

  /* back from dispatcher */
  roa_job_dispatcher_ctx_destroy(&ctx);

  if (ROA_IS_ENABLED(TEST_WITH_OUTPUT))
  {
    int i;

    printf("Test Data \n");

    for (i = 0; i < BATCH_COUNT; ++i)
    {
      printf("%d, ", test_data[i]);
    }

    printf("\n");
  }

  /* check test data is correct */
  {
    int i;

    for (i = 0; i < BATCH_COUNT; ++i)
    {
      unsigned expected = TICK_COUNT;
      ROA_ASSERT(test_data[i] == expected);
    }
  }

  return 0;
}
