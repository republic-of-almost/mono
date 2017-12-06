#include <mutex.hpp>
#include <config.hpp>

#ifndef _WIN32
#include <pthread.h>
#else
#define WIN_LEAN_AND_MEAN
#include <windows.h>
#endif


/* -------------------------------------------------------- [ Mutex Type ] -- */


struct optio_mutex
{
  #ifndef _WIN32
  pthread_mutex_t internal_mutex;
  #else
  HANDLE internal_mutex;
  #endif
};


/* ---------------------------------------------------- [ Mutex Lifetime ] -- */


void
optio_mutex_create(struct optio_mutex **mutex)
{
  const int bytes = sizeof(struct optio_mutex);
  struct optio_mutex *new_mut = (struct optio_mutex*)FIBER_MALLOC(bytes);

  #ifndef _WIN32  
  pthread_mutex_init(&new_mut->internal_mutex, NULL);  
  #else
  new_mut->internal_mutex = CreateMutex(NULL, FALSE, NULL);
  #endif

  *mutex = new_mut;
}


void
optio_mutex_destroy(struct optio_mutex **mutex)
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
optio_mutex_lock(struct optio_mutex *mutex)
{ 
  #ifndef _WIN32
  pthread_mutex_lock(&mutex->internal_mutex);
  #else
  WaitForSingleObject(mutex->internal_mutex, INFINITE);
  #endif
}


void
optio_mutex_unlock(struct optio_mutex *mutex)
{
  #ifndef _WIN32
  pthread_mutex_unlock(&mutex->internal_mutex);
  #else
  ReleaseMutex(mutex->internal_mutex);
  #endif
}

