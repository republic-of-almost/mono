
#include <coretech/window.h>
#include <stdio.h>
#include <dlfcn.h>

int
main()
{
  /* search for and load modules */
  {
    void *handle;
    
    void (*reg)();
    
    char *error;

    handle = dlopen ("libMonocle.dylib", RTLD_NOW);
    
    if (!handle)
    {
      fputs (dlerror(), stderr);
      return 0;
    }

    reg = dlsym(handle, "ct_module_register");
    if ((error = dlerror()) != NULL)
    {
      fputs(error, stderr);
      return 0;
    }

    reg();
    dlclose(handle);
  }

  /* */

  uint64_t window_id = ct_window_create();
  ct_window_set_width(window_id, 800);
  ct_window_set_height(window_id, 480);

  return 0;
}
