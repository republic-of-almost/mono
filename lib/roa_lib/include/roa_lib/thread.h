#ifndef THREAD_INCLUDED_620C4CB9_916F_4A5E_8389_DC5D456E21A5
#define THREAD_INCLUDED_620C4CB9_916F_4A5E_8389_DC5D456E21A5


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------ [ Thread Types ] -- */


typedef void* roa_thread;
typedef void* roa_thread_id;

typedef void*(*roa_thread_func)(void *);


/* --------------------------------------------------- [ Thread Lifetime ] -- */
/*
  Thread lifetime create and destroy.
*/


roa_thread
roa_thread_create(
	roa_thread_func func,
	void *arg,
	int stack_size,
	int core_affinity);


roa_thread
roa_thread_create_self();


void
roa_thread_destroy(roa_thread th);


/* ---------------------------------------------------- [ Thread Actions ] -- */
/*
  Various actions
*/


void
roa_thread_set_affinity(roa_thread th, int affinity);


void
roa_thread_join(roa_thread th);


/* -------------------------------------------------- [ Thread Utilities ] -- */


void
roa_thread_exit_current();


roa_thread_id
roa_thread_get_current_id();


void
roa_thread_set_current_name(const char *name);


void
roa_thread_get_current_name(char *buffer, unsigned buf_size);


unsigned
roa_thread_core_count();


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
