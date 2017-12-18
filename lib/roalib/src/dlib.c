#include <roalib/dlib.h>


#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#else
#include <dlfcn.h>
#endif


void*
roa_dlib_open(
  const char *name)
{
  #ifdef _WIN32
  const TCHAR *win_char(name);
  return (void*)LoadLibrary(win_char);
  #else
  dlopen(name, RTLD_LAZY);
  #endif
}


void*
roa_dlib_get_address(
  void *handle,
  const char *name)
{
  #ifdef _WIN32
  return GetProcAddress((HMODULE)handle, name);
  #else
  return dlsym(handle, name);
  #endif
}


void
roa_dlib_close(
  void *handle)
{
  #ifdef _WIN32
  FreeLibrary((HMODULE)handle);
  #else
  dlclose(handle);
  #endif
}