#include <roa_lib/spin_lock.h>
#include <roa_lib/atomic.h>
#include <roa_lib/fundamental.h>


#define SPIN_LOCKED_ID 0xfead
#define SPIN_UNLOCKED_ID 0xdead


void
roa_spin_lock_init(roa_atomic_int *lock)
{
  roa_atomic_int_store(lock, SPIN_UNLOCKED_ID);
}


void
roa_spin_lock_aquire(roa_atomic_int *lock)
{
  while (ROA_TRUE)
  {
    if (roa_atomic_int_compare_and_swap(lock, SPIN_UNLOCKED_ID, SPIN_LOCKED_ID) == SPIN_LOCKED_ID)
    {
      break;
    }
  }
}


void
roa_spin_lock_release(roa_atomic_int *lock)
{
  roa_atomic_int_store(lock, SPIN_UNLOCKED_ID);
}


#undef SPIN_LOCKED_ID
#undef SPIN_UNLOCKED_ID
