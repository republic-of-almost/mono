#include <app_data/app_data.hpp>
#include <roa_job/dispatcher.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_lib/array.h>


/* ----------------------------------------------- [ Shorts Entry Point ] -- */
/*
  Main should only create sub systems and start the dispatcher.
  Anything more and you should re-think design.
*/
int
main(int argc, char *argv[])
{
  /* create application data */
  app_data data{};
  roa_array_create(data.logic_updates, 16);

  /* create game systems */
  roa_dispatcher_create(&data.dispatcher, ROA_NULL);
  

  /* create inital frame job - this will self submit */
  roa_job_desc desc {
    app_setup,
    (void*)&data,
    ROA_TRUE,
  };
  
  roa_dispatcher_add_jobs(data.dispatcher, &desc, 1);
  roa_dispatcher_run(data.dispatcher);

  /* destroy game systems in reverse */
  roa_ctx_destroy(&data.hw_ctx);
  roa_dispatcher_destroy(&data.dispatcher);

  /* destroy application data */
  roa_array_destroy(data.logic_updates);

  return 0;
}
