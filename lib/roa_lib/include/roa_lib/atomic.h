#ifndef ATOMIC_INCLUDED_E1ECD72F_192A_4EB2_B939_1E4AD5E8B964
#define ATOMIC_INCLUDED_E1ECD72F_192A_4EB2_B939_1E4AD5E8B964


#ifdef __cplusplus
extern "C" {
#endif


/* ------------------------------------------------------ [ Atomic Types ] -- */


typedef union roa_atomic_int 
{
	void* align;
	int val;
} roa_atomic_int;


typedef union roa_atomic_ptr 
{
	void* ptr;
} roa_atomic_ptr;


/* ---------------------------------------------- [ Atomic Int Interface ] -- */


int
roa_atomic_int_load(
	roa_atomic_int *atomic);


void
roa_atomic_int_store(
	roa_atomic_int *atomic,
	int val);


int
roa_atomic_int_inc(
	roa_atomic_int *atomic);


int
roa_atomic_int_dec(
	roa_atomic_int *atomic);


int
roa_atomic_int_add(
	roa_atomic_int *atomic,
	int add);


int
roa_atomic_int_sub(
	roa_atomic_int *atomic,
	int sub);


int
roa_atomic_int_swap(
	roa_atomic_int *atomic,
	int swap);


int
roa_atomic_int_compare_and_swap(
	roa_atomic_int *atomic,
	int old_value,
	int new_value);


/* ---------------------------------------------- [ Atomic Ptr Interface ] -- */


void*
roa_atomic_ptr_load(
	roa_atomic_ptr *tomic);


void
roa_atomic_ptr_store(
	roa_atomic_ptr *atomic,
	void *value);


void*
roa_atomic_ptr_swap(
	roa_atomic_ptr *atomic,
	void *value);


void*
roa_atomic_ptr_compare_and_swap(
	roa_atomic_ptr *atomic,
	void *old_value,
	void *new_value);


#ifdef __cplusplus
} /* extern */
#endif


#endif /* inc guard */