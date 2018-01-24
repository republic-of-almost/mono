#ifndef MUTEX_INCLUDED_98AA2690_6EDF_4BDB_81EC_5B941FC8BC3F
#define MUTEX_INCLUDED_98AA2690_6EDF_4BDB_81EC_5B941FC8BC3F


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------------------- [ Mutex Type ] -- */


typedef void* roa_mutex;


/* ---------------------------------------------------- [ Mutex Lifetime ] -- */


roa_mutex
roa_mutex_create();


void
roa_mutex_destroy(roa_mutex *m);


/* ----------------------------------------------------- [ Mutex Actions ] -- */


void
roa_mutex_lock(roa_mutex m);


void
roa_mutex_unlock(roa_mutex m);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */