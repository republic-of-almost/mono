#include <rep/rep_api.h>
#include <data/engine_data.h>


/* ------------------------------------------------------------- [ Input ] -- */


void
rep_input_get(
  struct rep_input_desc *out_desc)
{
  *out_desc = *rep_data_input_data();
}