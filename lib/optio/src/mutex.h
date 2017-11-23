#ifndef MUTEX_INCLUDED_BBF03321_0837_48DA_9AD0_B1E4B9B1991C
#define MUTEX_INCLUDED_BBF03321_0837_48DA_9AD0_B1E4B9B1991C


/* --------------------------------------------------------- [ Mutex Fwd ] -- */


struct optio_mutex;


/* ---------------------------------------------------- [ Mutex Lifetime ] -- */


void
optio_mutex_create(struct optio_mutex **mutex);


void
optio_mutex_destroy(struct optio_mutex **mutex);


/* -------------------------------------------------------- [ Mutex Lock ] -- */


void
optio_mutex_lock(struct optio_mutex *mutex);


void
optio_mutex_unlock(struct optio_mutex *mutex);


#endif /* inc guard */
