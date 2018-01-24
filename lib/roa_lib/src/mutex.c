#include <roa_lib/mutex.h>
#include <roa_lib/fundamental.h>
#include <stdlib.h>


#if defined(__linux__) || defined(__APPLE__)
#include <pthread.h>
#elif defined(_WIN32)
#include <windows.h>
#endif


/* ---------------------------------------------------- [ Mutex Lifetime ] -- */


roa_mutex
roa_mutex_create()
{
	#if defined(__linux__) || defined(__APPLE__)
	pthread_mutex_t *m = malloc(sizeof(*m));
	pthread_mutex_init(m, NULL);
	
	return (roa_mutex)m;

	#elif defined(_WIN32)

	#endif
}


void
roa_mutex_destroy(roa_mutex *m)
{
	#if defined(__linux__) || defined(__APPLE__)
	pthread_mutex_destroy((pthread_mutex_t*) *m);
	free(*m);
	#elif defined(_WIN32)
	
	#endif
}


/* ----------------------------------------------------- [ Mutex Actions ] -- */


void
roa_mutex_lock(roa_mutex m)
{
	#if defined(__linux__) || defined(__APPLE__)
	pthread_mutex_lock((pthread_mutex_t*)m);
	#elif defined(_WIN32)
	
	#endif
}


void
roa_mutex_unlock(roa_mutex m)
{
	#if defined(__linux__) || defined(__APPLE__)
	pthread_mutex_unlock((pthread_mutex_t*)m);
	#elif defined(_WIN32)
	
	#endif
}