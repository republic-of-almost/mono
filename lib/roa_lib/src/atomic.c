#include <roa_lib/atomic.h>


#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(__clang__) || defined(__GNUC__)
#warning "Denny use yet"
#endif


/* ---------------------------------------------- [ Atomic Int Interface ] -- */


int
roa_atomic_int_load(
	roa_atomic_int *atomic)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_fetch_and_add(&atomic->val, 0);
	#elif defined(_WIN32)
	return InterlockedCompareExchange(&atomic->val, 0, 0);
	#endif
}


void
roa_atomic_int_store(
	roa_atomic_int *atomic,
	int val)
{
	#if defined(__clang__) || defined(__GNUC__)

	/*
	int ai_was = atomic->val;
	__sync_val_compare_and_swap(&atomic->val, ai_was, val);
	*/

	/*
  __sync_lock_test_and_set(&atomic->val, val);
	__sync_lock_release(&atomic->val);
	*/

	/* So aligned writes are atomic right? */
	atomic->val = val;

	#elif defined(_WIN32)
	InterlockedExchange(&atomic->val, val);
	#endif
}


int
roa_atomic_int_inc(
	roa_atomic_int *atomic)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_fetch_and_add(&atomic->val, 1);
	#elif defined(_WIN32)
  return InterlockedIncrement(&atomic->val) - 1;
	#endif
}


int
roa_atomic_int_dec(
	roa_atomic_int *atomic)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_fetch_and_sub(&atomic->val, 1);
	#elif defined(_WIN32)
  return InterlockedDecrement(&atomic->val) + 1;
	#endif
}


int
roa_atomic_int_add(
	roa_atomic_int *atomic,
	int add)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_fetch_and_add(&atomic->val, add);
	#elif defined(_WIN32)
  return InterlockedExchangeAdd(&atomic->val, add);
	#endif
}


int
roa_atomic_int_sub(
	roa_atomic_int *atomic,
	int sub)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_fetch_and_sub(&atomic->val, sub);
	#elif defined(_WIN32)
  return InterlockedExchangeAdd(&atomic->val, -sub);
	#endif
}


int
roa_atomic_int_swap(
	roa_atomic_int *atomic,
	int swap)
{
	#if defined(__clang__) || defined(__GNUC__)

	/* aligned read writes are atomic right */
	/* this isn't atomic however */
	int old = atomic->val;
	atomic->val = swap;	

	return old;

  /*int old = (int)__sync_lock_test_and_set(&atomic->val, swap);*/
  /*__sync_lock_release(&atomic->val);*/
	return old;
	#elif defined(_WIN32)
  return InterlockedExchange(&atomic->val, swap);
	#endif
}


int
roa_atomic_int_compare_and_swap(
	roa_atomic_int *atomic,
	int old_value,
	int new_value)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_val_compare_and_swap(&atomic->val, old_value, new_value);
	#elif defined(_WIN32)
  return InterlockedCompareExchange(&atomic->val, old_value, new_value);
	#endif
}


/* ---------------------------------------------- [ Atomic Ptr Interface ] -- */


void*
roa_atomic_ptr_load(
	roa_atomic_ptr *tomic)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}


void
roa_atomic_ptr_store(
	roa_atomic_ptr *atomic,
	void *value)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}


void*
roa_atomic_ptr_swap(
	roa_atomic_ptr *atomic,
	void *value)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}


void*
roa_atomic_ptr_compare_and_swap(
	roa_atomic_ptr *atomic,
	void *old_value,
	void *new_value)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}

