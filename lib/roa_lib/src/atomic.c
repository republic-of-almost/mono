#include <roa_lib/atomic.h>
#include <stdio.h>

/* ---------------------------------------------- [ Atomic Int Interface ] -- */


int
roa_atomic_int_load(
	roa_atomic_int *atomic)
{
	#if defined(__clang__) || defined(__GNUC__)
	return (int)__sync_fetch_and_add(&atomic->val, 0);
	#elif defined(_WIN32)
	return InterlockedCompareExchange(&atomic->i, 0, 0);
	#endif
}


void
roa_atomic_int_store(
	roa_atomic_int *atomic,
	int val)
{
	printf("VAL: %d\n", val);

	#if defined(__clang__) || defined(__GNUC__)

	int ai_was = atomic->val;
	__sync_val_compare_and_swap(&atomic->val, ai_was, val);


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

	#endif
}


int
roa_atomic_int_dec(
	roa_atomic_int *atomic)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}


int
roa_atomic_int_add(
	roa_atomic_int *atomic,
	int add)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}


int
roa_atomic_int_sub(
	roa_atomic_int *atomic,
	int sub)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}


int
roa_atomic_int_swap(
	roa_atomic_int *atomic,
	int swap)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

	#endif
}


int
roa_atomic_int_compare_and_swap(
	roa_atomic_int *atomic,
	int old_value,
	int new_value)
{
	#if defined(__clang__) || defined(__GNUC__)
	
	#elif defined(_WIN32)

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

