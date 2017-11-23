#include <mutex.h>
#include <config.h>
#include <pthread.h>


/* -------------------------------------------------------- [ Mutex Type ] -- */


struct optio_mutex
{
  pthread_mutex_t internal_mutex;
};


/* ---------------------------------------------------- [ Mutex Lifetime ] -- */


void
optio_mutex_create(struct optio_mutex **mutex)
{
  const int bytes = sizeof(struct optio_mutex);
  struct optio_mutex *new_mut = (struct optio_mutex*)FIBER_MALLOC(bytes);
  
  pthread_mutex_init(&new_mut->internal_mutex, NULL);
  
  *mutex = new_mut;
}


void
optio_mutex_destroy(struct optio_mutex **mutex)
{
  pthread_mutex_destroy(&(*mutex)->internal_mutex);
  
  FIBER_FREE(*mutex);
}


/* -------------------------------------------------------- [ Mutex Lock ] -- */


void
optio_mutex_lock(struct optio_mutex *mutex)
{
  pthread_mutex_lock(&mutex->internal_mutex);
}


void
optio_mutex_unlock(struct optio_mutex *mutex)
{
  pthread_mutex_unlock(&mutex->internal_mutex);
}

