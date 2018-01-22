#ifndef MUTEX_INCLUDED_BBF03321_0837_48DA_9AD0_B1E4B9B1991C
#define MUTEX_INCLUDED_BBF03321_0837_48DA_9AD0_B1E4B9B1991C


/* --------------------------------------------------------- [ Mutex Fwd ] -- */


struct roa_mutex;


/* ---------------------------------------------------- [ Mutex Lifetime ] -- */


void
roa_mutex_create(struct roa_mutex **mutex);


void
roa_mutex_destroy(struct roa_mutex **mutex);


/* -------------------------------------------------------- [ Mutex Lock ] -- */


void
roa_mutex_lock(struct roa_mutex *mutex);


void
roa_mutex_unlock(struct roa_mutex *mutex);


#endif /* inc guard */
