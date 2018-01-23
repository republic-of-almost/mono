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
#elif defined(_WIN32)

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

  #else
  #error "Unsupported platform"
  #endif

  return ROA_NULL;
}


roa_thread
roa_thread_create_self()
{
  #if defined(__linux__) || defined(__APPLE__)
  return (roa_thread)pthread_self();
  #elif defined(_WIN32)

  #else
  #error "Unsupported Platform"
  #endif

  return ROA_NULL;
}


void
roa_thread_destroy(roa_thread *th)
{
  #if defined(__linux__) || defined(__APPLE__)
  int success = pthread_join((pthread_t)*th, ROA_NULL);

  if(success == 0)
  {
    *th = ROA_NULL;
  }

  #elif defined(_WIN32)

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
  pthread_join((pthread_t) th, NULL);
  #elif defined(_WIN32)

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
  ROA_ASSERT(success);
  #elif defined(_WIN32)
  /* windows method */
  #else
  #error "Unsupported Platform"
  #endif
}


void
roa_thread_get_current_name(char *buffer, unsigned buffer_size)
{
  #if defined(__linux__)
  int success = pthread_getname_np(pthread_self(), buffer, buffer_size);
  ROA_ASSERT(success == 0);
  #elif defined(__APPLE__)
  int success = pthread_setname_np(name);
  ROA_ASSERT(success);
  #elif defined(_WIN32)
  /* windows method */
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

  #elif defined(_WIN32)

  #else
  #error "Unsupported Platform"
  #endif

  return 0;
}



