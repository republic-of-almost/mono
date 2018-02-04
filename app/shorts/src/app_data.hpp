#ifndef APP_DATA_57DE7621_03E4_412F_9374_8657614310CB
#define APP_DATA_57DE7621_03E4_412F_9374_8657614310CB


/* ------------------------------------------------- [ Application Data ] -- */


typedef struct roa_ctx *roa_ctx_t;
typedef struct roa_dispatcher_ctx *roa_dispatcher_ctx_t;

struct roa_job_desc;


struct app_data
{
  /* systems */
  roa_ctx_t hw_ctx;
  roa_dispatcher_ctx_t dispatcher;

  /* logic */
  /* array */ roa_job_desc *logic_updates;
};


#endif /* inc data */
