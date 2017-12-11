#ifndef COUNTER_INCLUDED_4C570976_30BE_4E64_BD6F_D1C66A221B99
#define COUNTER_INCLUDED_4C570976_30BE_4E64_BD6F_D1C66A221B99


#include <atomic>


struct optio_counter
{
  int batch_id;
  int has_pending;
  std::atomic_int value;
};


/* ------------------------------------------------ [ Counter Attributes ] -- */


void
optio_counter_set(struct optio_counter *counter, int value, int batch_id);


void
optio_counter_decrement(struct optio_counter *counter);


int
optio_counter_value(struct optio_counter *counter);


#endif /* inc guard */