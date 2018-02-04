#include <app_data.hpp>
#include <states/main_menu.hpp>
#include <roa_job/dispatcher.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/array.h>


ROA_JOB_DECL(app_setup)
ROA_JOB_DECL(app_frame)


/* -------------------------------------------- [ Application Frame Job ] -- */


/*
  Submits a new frame to keep the dispatcher alive.
  Must always be at least one in the system or the application will quit.
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
ROA_JOB(app_frame, struct app_data *)
{
  ROA_ASSERT(arg);
  ROA_ASSERT_PEDANTIC(job_ctx);

  /* game logic tick */
  {
    const unsigned count = roa_array_size(arg->logic_updates);
    const roa_job_desc *data = arg->logic_updates;

    const unsigned logic_counter = roa_dispatcher_add_jobs(
      job_ctx,
      data,
      count
    );
    
    roa_dispatcher_wait_for_counter(job_ctx, logic_counter);
  }

  /* game system tick */
  {
    
  }

  /* submit the next frame if we still have a context */
  {
    const ROA_BOOL new_frame = roa_ctx_new_frame(arg->hw_ctx);

    if (new_frame)
    {
      app_submit_frame(arg->dispatcher, arg);
    }
  }
}


/*
  Inital setup of application
*/
ROA_JOB(app_setup, struct app_data *)
{
  /* setup things we need */


  /* start the game loop */
  app_submit_frame(job_ctx, arg);
}


/* ----------------------------------------------- [ Shorts Entry Point ] -- */
/*
  Main should only create sub systems and start the dispatcher.
  Anything more and you should re-think design.
*/
int
main()
{
  /* create application data */
  app_data data{};
  roa_array_create(data.logic_updates, 16);

  /* create game systems */
  roa_dispatcher_create(&data.dispatcher, ROA_NULL);
  roa_ctx_create(&data.hw_ctx);

  /* create inital frame job - this will self submit */
  
  roa_dispatcher_run(data.dispatcher);

  /* destroy game systems in reverse */
  roa_ctx_destroy(&data.hw_ctx);
  roa_dispatcher_destroy(&data.dispatcher);

  /* destroy application data */
  roa_array_destroy(data.logic_updates);

  return 0;
}