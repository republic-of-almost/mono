#include <rep/rep_api.h>
#include <data/engine_data.h>


/* -------------------------------------------------------------- [ Time ] -- */


void
rep_time_get(
  struct rep_time_desc * out_desc)
{
  *out_desc = *rep_data_time();
}