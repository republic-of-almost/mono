#include <roa_lib/sys.h>

#ifdef _WIN32
#include <windows.h>
#endif


int
roa_cpu_count()
{
  #ifdef _WIN32
  /* todo hyperthread check */
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  int cpu_count = sysinfo.dwNumberOfProcessors;
  return cpu_count;
  #else
  return 0;
  #endif
}