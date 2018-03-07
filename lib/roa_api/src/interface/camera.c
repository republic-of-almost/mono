#include <rep/rep_api.h>
#include <roa_lib/assert.h>


void
rep_camera_set(
  uint32_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);
}


void
rep_camera_get(
  const uint32_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);
}