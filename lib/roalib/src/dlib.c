#include <roalib/dlib.h>


#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#else
#include <dlfcn.h>
#endif


HMODULE
roa_internal_dlib_open_window(const TCHAR *name)
{
  return LoadLibrary(name);
}


void*
roa_dlib_open(
  const char *name)
{
  #ifdef _WIN32
  return (void*)roa_internal_dlib_open_window(name);
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