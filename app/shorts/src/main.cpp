#include <roa_job/dispatcher.h>
#include <roa_ctx/roa_ctx.h>


struct short_app_data
{
  roa_ctx_t hw_ctx;
};


/* ----------------------------------------------- [ Shorts Entry Point ] -- */


int
main()
{
  short_app_data data;

  roa_ctx_create(&data.hw_ctx);

  ROA_BOOL new_frame = ROA_TRUE;

  while (new_frame)
  {
    new_frame = roa_ctx_new_frame(data.hw_ctx);
  }

  roa_ctx_destroy(&data.hw_ctx);

  return 0;
}