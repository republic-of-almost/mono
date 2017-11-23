#ifndef THREAD_INCLUDED_620C4CB9_916F_4A5E_8389_DC5D456E21A4
#define THREAD_INCLUDED_620C4CB9_916F_4A5E_8389_DC5D456E21A4


/* ---------------------------------------------------- [ Thread Fwd Dec ] -- */


struct optio_thread;
typedef void*(*thread_func)(void*);


/* ------------------------------------------------------ [ Thread Types ] -- */


struct optio_thread_desc
{
  thread_func func;
  void *arg;
  int affinity;
};


/* --------------------------------------------------- [ Thread Lifetime ] -- */
/*
  Thread lifetime create and destroy.
*/

void
optio_thread_create(struct optio_thread **th, struct optio_thread_desc *desc);


struct optio_thread*
optio_thread_create_this();


void
optio_thread_destroy(struct optio_thread **th);


/* ---------------------------------------------------- [ Thread Actions ] -- */
/*
  Various actions
*/


int
optio_thread_find_this(struct optio_thread **th, unsigned count);


void
optio_thread_join(struct optio_thread **th, unsigned count);


int
optio_thread_core_count();


#endif /* inc guard */
