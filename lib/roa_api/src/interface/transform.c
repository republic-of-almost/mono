#include <rep/rep_api.h>
#include <roa_lib/assert.h>


void
rep_transform_set(
  uint64_t * object_ids,
  struct rep_transform_desc *desc,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);


}


void
rep_transform_get(
  const uint64_t * object_ids,
  struct rep_transform_desc *desc,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);


}