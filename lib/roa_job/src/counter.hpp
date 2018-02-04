#ifndef COUNTER_INCLUDED_4C570976_30BE_4E64_BD6F_D1C66A221B99
#define COUNTER_INCLUDED_4C570976_30BE_4E64_BD6F_D1C66A221B99


#include <atomic>


struct roa_counter
{
  int batch_id;
  int has_pending;
  int thread_id;
  std::atomic_int value;
};


/* ------------------------------------------------ [ Counter Attributes ] -- */


void
roa_counter_set(struct roa_counter *counter, int value, int batch_id);


void
roa_counter_decrement(struct roa_counter *counter);


int
roa_counter_value(struct roa_counter *counter);


#endif /* inc guard */
