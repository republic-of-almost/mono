#include <rep/rep_api.h>
#include <roa_lib/assert.h>


unsigned
rep_submit_tasks(
  rep_task *tasks,
  unsigned count)
{
  ROA_ASSERT(tasks);
  ROA_ASSERT(count);

  return 0;
}


void
rep_wait_for_tasks(
  unsigned marker)
{
  ROA_ASSERT(marker);
}