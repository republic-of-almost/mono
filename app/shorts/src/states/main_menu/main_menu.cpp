#include <states/main_menu.hpp>
#include <app_data/app_data.hpp>


namespace {


};


ROA_JOB(main_menu_setup, app_data*)
{
  ROA_UNUSED(arg);
  ROA_UNUSED(job_ctx);

  /* setup menu stuff */
  


  /* add main menu tick */
  {
  }
}


ROA_JOB(main_menu_tick, app_data*)
{
  ROA_UNUSED(arg);
  ROA_UNUSED(job_ctx);

  /* tick menu stuff */

  /* do stuff submit to renderer */
  {
    
  }
}


ROA_JOB(main_menu_shutdown, app_data*)
{
  ROA_UNUSED(arg);
  ROA_UNUSED(job_ctx);

  /* shutdown menu stuff */
  
}

