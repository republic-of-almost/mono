#include <counter.hpp>


/* ----------------------------------------------- [ Lifetime Attributes ] -- */


void
roa_counter_set(struct roa_counter *counter, int value, int batch_id)
{
  counter->batch_id = batch_id;
  counter->has_pending = 0;
  atomic_store(&counter->value, value);
}


void
roa_counter_decrement(struct roa_counter *counter)
{
  atomic_fetch_sub(&counter->value, 1);
}


int
roa_counter_value(struct roa_counter *counter)
{
  int value = atomic_load(&counter->value);
  
  return value;
}
