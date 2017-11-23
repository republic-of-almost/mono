#ifndef FIBER_INCLUDED_4FF59572_11DC_4131_AEF6_7E7686AB5A20
#define FIBER_INCLUDED_4FF59572_11DC_4131_AEF6_7E7686AB5A20


/* ------------------------------------------------------- [ Fiber Types ] -- */
/*
  A fiber represents and execution context, these can be freely switched with
  minimal performance overhead.
*/


struct optio_fiber;

typedef void(*optio_fiber_func)(void*);


/* ---------------------------------------------------- [ Fiber Lifetime ] -- */
/*
  Create a fiber by passing it a function to execute. If you wish a fiber that
  represents the local context simply pass 0/NULL in the 'func' paramater in
  the create function.
*/


void
optio_fiber_create(struct optio_fiber **fiber, optio_fiber_func func, void *arg);


void
optio_fiber_destroy(struct optio_fiber *fiber);


/* -------------------------------------------------- [ Fiber Attributes ] -- */


void
optio_fiber_set_user_id(struct optio_fiber *fiber, unsigned user_id);


unsigned
optio_fiber_get_user_id(const struct optio_fiber *fiber);


void
optio_fiber_set_block_id(struct optio_fiber *fiber, unsigned block_id);


unsigned
optio_fiber_get_block_id(const struct optio_fiber *fiber);


/* ----------------------------------------------------- [ Fiber Actions ] -- */
/*
  Switch from one execution context to another context. Both contexts must be
  valid.
*/


void
optio_fiber_switch(struct optio_fiber *this_fiber, struct optio_fiber *new_fiber);


#endif /* inc guard */
