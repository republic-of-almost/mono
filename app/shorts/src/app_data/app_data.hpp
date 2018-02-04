#ifndef APP_DATA_57DE7621_03E4_412F_9374_8657614310CB
#define APP_DATA_57DE7621_03E4_412F_9374_8657614310CB


#include <roa_job/dispatcher.h>


/* ------------------------------------------------- [ Application Data ] -- */


typedef struct roa_ctx *roa_ctx_t;
struct roa_job_desc;


struct app_data
{
  /* systems */
  roa_ctx_t hw_ctx;
  roa_dispatcher_ctx_t dispatcher;

  /* logic */
  /* array */ roa_job_desc *logic_updates;
};


ROA_JOB_DECL(app_setup)
ROA_JOB_DECL(app_frame)


#endif /* inc data */
