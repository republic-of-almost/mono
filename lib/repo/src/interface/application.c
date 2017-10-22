#include <repo/application.h>
#include <engine/engine.h>


/* ---------------------------------------------- [ Application Lifetime ] -- */


void
repo_app_tick()
{
  /* allocate some space */

  /* startup engine components */
  repo_engine_create();
}


void
repo_app_tick(void *user_data, repo_tick_callback cb)
{
  /* tick */
  while(repo_engine_tick())
  {
    /* user callback */
    if(cb)
    {
      cb(user_data);   
    }
  }
}


void
repo_app_destroy()
{
  /* destroy engine compoents */
  repo_engine_destroy();
}
