
#include <coretech/window.h>
#include <stdio.h>
#include <dlfcn.h>


typedef void(*module_callback)();

int
main()
{
  module_callback reg = NULL;
  module_callback tick = NULL;
  void *handle;

  /* search for and load modules */
  {
    
    
    char *error;

    handle = dlopen ("libMonocle.dylib", RTLD_NOW);
    
    if (!handle)
    {
      fputs (dlerror(), stderr);
      return 0;
    }
    
    tick = dlsym(handle, "ct_module_tick");
    if ((error = dlerror()) != NULL)
    {
      fputs(error, stderr);
      return 0;
    }

    reg = dlsym(handle, "ct_module_register");
    if ((error = dlerror()) != NULL)
    {
      fputs(error, stderr);
      return 0;
    }
  
    reg();
    tick();
  }

  /* */

  uint64_t window_id = ct_window_create();
  ct_window_set_width(window_id, 800);
  ct_window_set_height(window_id, 480);
  
  while(true)
  {
    if(tick)
    {
      tick();
    }
  }
  
  /* dl close */
  dlclose(handle);

  return 0;
}
