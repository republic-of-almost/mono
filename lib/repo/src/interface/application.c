#include <repo/application.h>
#include <engine/engine.h>


/* ---------------------------------------------- [ Application Lifetime ] -- */


void
repo_app_tick()
{
  /* allocate some space */

  /* startup engine components */
  repo_engine_create(0);
}


void
repo_app_destroy()
{
  /* destroy engine compoents */
  repo_engine_destroy(0);
}
