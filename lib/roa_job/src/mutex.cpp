#include <mutex.hpp>
#include <config.hpp>

#ifndef _WIN32
#include <pthread.h>
#else
#define WIN_LEAN_AND_MEAN
#include <windows.h>
#endif


/* -------------------------------------------------------- [ Mutex Type ] -- */


struct roa_mutex
{
  #ifndef _WIN32
  pthread_mutex_t internal_mutex;
  #else
  HANDLE internal_mutex;
  #endif
};


/* ---------------------------------------------------- [ Mutex Lifetime ] -- */


void
roa_mutex_create(struct roa_mutex **mutex)
{
  const int bytes = sizeof(struct roa_mutex);
  struct roa_mutex *new_mut = (struct roa_mutex*)FIBER_MALLOC(bytes);

  #ifndef _WIN32  
  pthread_mutex_init(&new_mut->internal_mutex, NULL);  
  #else
  new_mut->internal_mutex = CreateMutex(NULL, FALSE, NULL);
  #endif

  *mutex = new_mut;
}


void
roa_mutex_destroy(struct roa_mutex **mutex)
{
  #ifndef _WIN32
  pthread_mutex_destroy(&(*mutex)->internal_mutex);
  #else
  CloseHandle((*mutex)->internal_mutex);
  #endif
  
  FIBER_FREE(*mutex);
}


/* -------------------------------------------------------- [ Mutex Lock ] -- */


void
roa_mutex_lock(struct roa_mutex *mutex)
{ 
  #ifndef _WIN32
  pthread_mutex_lock(&mutex->internal_mutex);
  #else
  WaitForSingleObject(mutex->internal_mutex, INFINITE);
  #endif
}


void
roa_mutex_unlock(struct roa_mutex *mutex)
{
  #ifndef _WIN32
  pthread_mutex_unlock(&mutex->internal_mutex);
  #else
  ReleaseMutex(mutex->internal_mutex);
  #endif
}

