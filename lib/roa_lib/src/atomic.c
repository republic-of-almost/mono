#include <roa_lib/atomic.h>
#include <roa_lib/fundamental.h>


/* based off https://github.com/mattiasgustavsson/libs/blob/master/thread.h */


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
	return InterlockedCompareExchange((LONG*)&atomic->val, 0, 0);
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
	InterlockedExchange((LONG*)&atomic->val, val);
	#endif
}


int
roa_atomic_int_inc(
	roa_atomic_int *atomic)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_fetch_and_add(&atomic->val, 1) + 1;
	#elif defined(_WIN32)
  return InterlockedIncrement((LONG*)&atomic->val);
	#endif
}


int
roa_atomic_int_dec(
	roa_atomic_int *atomic)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_fetch_and_sub(&atomic->val, 1);
	#elif defined(_WIN32)
  return InterlockedDecrement((LONG*)&atomic->val) + 1;
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
  return InterlockedExchangeAdd((LONG*)&atomic->val, add);
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
  return InterlockedExchangeAdd((LONG*)&atomic->val, -sub);
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
  return InterlockedExchange((LONG*)&atomic->val, swap);
	#endif
}


int
roa_atomic_int_compare_and_swap(
	roa_atomic_int *atomic,
	int compare,
	int new_value)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_val_compare_and_swap(&atomic->val, compare, new_value);
	#elif defined(_WIN32)
  return InterlockedCompareExchange((LONG*)&atomic->val, new_value, compare);
	#endif
}


/* ---------------------------------------------- [ Atomic Ptr Interface ] -- */


void*
roa_atomic_ptr_load(
	roa_atomic_ptr *atomic)
{
  ROA_UNUSED(atomic);

	#if defined(__clang__) || defined(__GNUC__)
  return ROA_NULL;
	#elif defined(_WIN32)
  return ROA_NULL;
	#endif
}


void
roa_atomic_ptr_store(
	roa_atomic_ptr *atomic,
	void *value)
{
  ROA_UNUSED(atomic);
  ROA_UNUSED(value);

	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}


void*
roa_atomic_ptr_swap(
	roa_atomic_ptr *atomic,
	void *value)
{
  ROA_UNUSED(atomic);
  ROA_UNUSED(value);

	#if defined(__clang__) || defined(__GNUC__)
  return ROA_NULL;
	#elif defined(_WIN32)
  return ROA_NULL;
	#endif
}


void*
roa_atomic_ptr_compare_and_swap(
	roa_atomic_ptr *atomic,
	void *old_value,
	void *new_value)
{
  ROA_UNUSED(atomic);
  ROA_UNUSED(old_value);
  ROA_UNUSED(new_value);

	#if defined(__clang__) || defined(__GNUC__)
  return ROA_NULL;
	#elif defined(_WIN32)
  return ROA_NULL;
	#endif
}

