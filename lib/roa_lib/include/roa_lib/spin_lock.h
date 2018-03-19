#ifndef SPIN_LOCK_INCLUDED_4762A828_13CF_4464_8A84_B22858A52084
#define SPIN_LOCK_INCLUDED_4762A828_13CF_4464_8A84_B22858A52084


#include <roa_lib/fundamental.h>
#include <roa_lib/atomic.h>


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------- [ Spin Lock Actions ] -- */


void
roa_spin_lock_init(roa_atomic_int *lock);


void
roa_spin_lock_aquire(roa_atomic_int *lock);


ROA_BOOL
roa_spin_lock_try_aquire(roa_atomic_int *lock);


void
roa_spin_lock_release(roa_atomic_int *lock);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */
