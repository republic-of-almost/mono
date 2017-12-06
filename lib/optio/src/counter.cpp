#include <counter.hpp>


/* ----------------------------------------------- [ Lifetime Attributes ] -- */


void
optio_counter_set(struct optio_counter *counter, int value, int batch_id)
{
  counter->batch_id = batch_id;
  counter->has_pending = 0;
  atomic_store(&counter->value, value);
}


void
optio_counter_decrement(struct optio_counter *counter)
{
  atomic_fetch_sub(&counter->value, 1);
}


int
optio_counter_value(struct optio_counter *counter)
{
  int value = atomic_load(&counter->value);
  
  return value;
}
