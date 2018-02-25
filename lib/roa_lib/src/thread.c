#include <roa_lib/thread.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/assert.h>


#if defined(__linux__)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <pthread.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <pthread.h>
#include <sys/sysctl.h>
#elif defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <process.h>
const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack( push, 8 )
typedef struct tagTHREADNAME_INFO
{
  DWORD dwType;
  LPCSTR szName;
  DWORD dwThreadID;
  DWORD dwFlags;
} THREADNAME_INFO;
#pragma pack(pop)

#endif


/* --------------------------------------------------- [ Thread Lifetime ] -- */
/*
  Thread lifetime create and destroy.
*/


roa_thread
roa_thread_create(
  roa_thread_func func,
  void *arg,
  int stack_size,
  int core_affinity)
{
  #if defined(__linux__) || defined(__APPLE__)
  pthread_t th;

  int success = pthread_create(
    &th,
    NULL,
    func,
    arg
  );

  if(success != 0)
  {
    return ROA_NULL;
  }

  return (roa_thread)th;

  #elif defined(_WIN32)
  ROA_UNUSED(core_affinity);
  ROA_UNUSED(stack_size);

  HANDLE handle = (HANDLE)(_beginthreadex(
    NULL,
    524288,
    (_beginthreadex_proc_type)func,
    arg,
    0,
    NULL)
  );

  if(handle == 0)
  {
    return ROA_NULL;
  }

  return (roa_thread)handle;

  #else
  #error "Unsupported platform"
  #endif
}


roa_thread
roa_thread_create_self()
{
  #if defined(__linux__) || defined(__APPLE__)
  return (roa_thread)pthread_self();
  #elif defined(_WIN32)
  return (roa_thread)GetCurrentThreadId();
  #else
  #error "Unsupported Platform"
  #endif
}


void
roa_thread_destroy(roa_thread *th)
{
  ROA_ASSERT(th);

  #if defined(__linux__) || defined(__APPLE__)
  int success = pthread_join((pthread_t)*th, ROA_NULL);

  if(success == 0)
  {
    *th = ROA_NULL;
  }

  #elif defined(_WIN32)

  HANDLE win_th = (HANDLE)*th;

  WaitForSingleObject(win_th, INFINITE);
  CloseHandle(win_th);

  *th = ROA_NULL;
  #else
  #error "Unsupported Platform"
  #endif
}


/* ---------------------------------------------------- [ Thread Actions ] -- */
/*
  Various actions
*/


void
roa_thread_join(roa_thread th)
{
  #if defined(__linux__) || defined(__APPLE__)
  pthread_join((pthread_t)th, NULL);
  #elif defined(_WIN32)
  WaitForSingleObject((HANDLE)th, INFINITE);
  #else
  #error "Unsupported Platform"
  #endif
}


/* -------------------------------------------------- [ Thread Utilities ] -- */


void
roa_thread_exit_current()
{
  #if defined(__linux__) || defined(__APPLE__)
  pthread_exit(ROA_NULL);
  #elif defined(_WIN32)
  ExitThread(0);
  #else
  #error "Unsupported Platform";
  #endif
}


roa_thread_id
roa_thread_get_current_id()
{
  #if defined(__linux__) || defined(__APPLE__)
  return (roa_thread_id)pthread_self();
  #elif defined(_WIN32)
  return (roa_thread_id)GetCurrentThreadId();
  #else
  #error "Unsupported Platform"
  #endif
}


void
roa_thread_set_current_name(const char *name)
{
  #if defined(__linux__)
  int success = pthread_setname_np(pthread_self(), name);
  ROA_ASSERT(success == 0);
  #elif defined(__APPLE__)
  int success = pthread_setname_np(name);
  ROA_ASSERT(success == 0);
  #elif defined(_WIN32)
  {
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;

    __try
    {
      RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
  }
  #else
  #error "Unsupported Platform"
  #endif
}


unsigned
roa_thread_core_count()
{
  #if defined(__linux__)
  return sysconf(_SC_NPROCESSORS_ONLN);
  #elif defined(__APPLE__)
  unsigned count;
  size_t count_len = sizeof(count);
  sysctlbyname("hw.logicalcpu", &count, &count_len, NULL, 0);
  return count;
  #elif defined(_WIN32)
  SYSTEM_INFO sys_info;
  GetSystemInfo(&sys_info);
  return sys_info.dwNumberOfProcessors;
  #else
  #error "Unsupported Platform"
  #endif
}



