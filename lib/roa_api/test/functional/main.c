#include <rep/rep_api.h>
#include <roa_lib/fundamental.h>
#include <string.h>


enum {
  OBJ_CAMERA = 0,
  OBJ_ACTOR = 1,
  OBJ_RENDERABLES_START = 2,

  OBJ_COUNT = 128,
};


uint32_t objects[OBJ_COUNT];


void
rep_job_func(void *arg)
{

}


void
rep_frame_func(void *arg)
{
  struct rep_task_desc desc[1];

  desc[0].arg = 0;
  desc[0].function = rep_job_func;

  rep_task_submit(&desc[0], 1);
}


void
rep_func_setup(void *arg)
{
  /* setup application data */
  {
    struct rep_object_desc obj_desc[OBJ_COUNT];
    memset(obj_desc, 0, sizoef(*obj_desc));

    rep_object_create(&objects, ROA_NULL, OBJ_COUNT);

    
  }

  /* reset frame job */
  {
    struct rep_app_desc app_desc;
    rep_app_get(&app_desc);
    app_desc.frame_job = rep_frame_func;
    rep_app_set(&app_desc);
  }
}


int
main()
{
  /* create rep application */
  {
    struct rep_app_desc app_desc;
    app_desc.title = "Rep Func Test";
    app_desc.width = 1200;
    app_desc.height = 720;
    app_desc.frame_job = rep_func_setup;

    rep_app_create(&app_desc);
  }

  return 0;
}