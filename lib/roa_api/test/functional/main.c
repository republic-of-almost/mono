#include <rep/rep_api.h>


void
rep_func_test_frame(void *arg)
{
  /* quit straight away */
  rep_app_close();
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