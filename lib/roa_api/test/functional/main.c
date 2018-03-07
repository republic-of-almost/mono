#include <rep/rep_api.h>


void
rep_func_test_job(void *arg)
{

}


void
rep_func_test_frame(void *arg)
{
  struct rep_task_desc desc[1];
  
  desc[0].arg = 0;
  desc[0].function = rep_func_test_job;

  rep_task_submit(&desc[0], 1);
}


int
main()
{
  /* create rep application */
  struct rep_app_desc app_desc;
  app_desc.title = "Rep Func Test";
  app_desc.width = 1200;
  app_desc.height = 720;
  app_desc.frame_job = rep_func_test_frame;

  rep_app_create(&app_desc);

  return 0;
}