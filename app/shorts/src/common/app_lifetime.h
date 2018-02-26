#ifndef APP_LIFETIME_INCLUDED_CAB38F90_5FE6_4594_8846_A36ED4B67D2F
#define APP_LIFETIME_INCLUDED_CAB38F90_5FE6_4594_8846_A36ED4B67D2F


#include <roa_job/dispatcher.h>

struct shorts_app_data;

ROA_JOB_DECL(app_startup);


struct app_thinker_data_arg
{
  struct shorts_app_data *app_data;
  struct roa_job_desc desc;
};

ROA_JOB_DECL(app_add_thinker);


struct app_renderer_data_arg
{
  struct shorts_app_data *app_data;
  struct roa_job_desc desc;
};

ROA_JOB_DECL(app_add_renderer);


#endif /* inc guard */