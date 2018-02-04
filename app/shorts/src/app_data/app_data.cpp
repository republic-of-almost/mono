#include <app_data/app_data.hpp>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>
#include <roa_ctx/roa_ctx.h>
#include <states/main_menu.hpp>


/* -------------------------------------------- [ Application Frame Job ] -- */


/*
  Helper to submit a new frame. There must always be a frame in the system or
  the dispatcher will quit.
*/
void
app_submit_frame(roa_dispatcher_ctx_t job_ctx, app_data *arg)
{
  /* param check */
  ROA_ASSERT_PEDANTIC(job_ctx);
  ROA_ASSERT_PEDANTIC(arg);

  const roa_job_desc desc[] = {
    /* main tick frame */
    {
      app_frame,
      (void*)arg,
      ROA_TRUE,
    },
  };

  roa_dispatcher_add_jobs(
    job_ctx,
    ROA_ARR_DATA(desc),
    ROA_ARR_COUNT(desc)
  );
}



/*
  Happens every frame.
  This is the ubiquitous for(;;) game loop contents.
*/
ROA_JOB(app_frame, struct app_data*)
{
  ROA_ASSERT(arg);
  ROA_ASSERT_PEDANTIC(job_ctx);

  /* game logic tick */
  {
    const unsigned count = roa_array_size(arg->logic_updates);

    if (count)
    {
      const roa_job_desc *data = arg->logic_updates;
      ROA_ASSERT_PEDANTIC(data);

      const unsigned logic_counter = roa_dispatcher_add_jobs(
        job_ctx,
        data,
        count
      );

      roa_dispatcher_wait_for_counter(job_ctx, logic_counter);
    }
  }

  /* game systems tick */
  {
    const ROA_BOOL new_frame = roa_ctx_new_frame(arg->hw_ctx);

    if (new_frame)
    {
      /* game systems ticks */


      /* submit the next frame if we still have a context */
      app_submit_frame(arg->dispatcher, arg);
    }
  }
}


/*
Inital setup of application
*/
ROA_JOB(app_setup, struct app_data *)
{
  roa_ctx_create(&arg->hw_ctx);

  /* various setup jobs */
  {
    roa_job_desc setup_desc[] = {
      {
        main_menu_setup,
        (void*)arg,
        ROA_FALSE
      },
    };

    /* allow setup to start */
    const unsigned setup_marker = roa_dispatcher_add_jobs(
      job_ctx,
      ROA_ARR_DATA(setup_desc),
      ROA_ARR_COUNT(setup_desc)
    );

    /* wait for setup to complete */
    roa_dispatcher_wait_for_counter(job_ctx, setup_marker);
  }

  /* start the game loop */
  app_submit_frame(job_ctx, arg);
}