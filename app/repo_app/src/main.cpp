#include <optio/dispatcher.h>
#include <stdint.h>


void
application_setup()
{
};


void
frame(void *arg)
{
  repo_application_close();
}


void
repo_lib_startup()
{
  repo_job_set_frame(my_frame_job);
}


int
main()
{ 
  /* look for shared libs */
  {
    /* load shared libs */
  }
  
  /* call startup on shared libs */ 
  {
    /* startup libs */
  }

  /* run application */
  {
    
  }

  /* shutdown shared libs */
  {
    /* shutdown each lib */
  }
  return 0;
}
