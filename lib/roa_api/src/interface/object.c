#include <rep/rep_api.h>
#include <roa_lib/assert.h>


void
rep_object_create(
  uint32_t * out_object_ids,
  const struct rep_object_desc * desc,
  unsigned count)
{
  ROA_ASSERT(out_object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);

}


void
rep_object_update(
  const uint32_t * object_ids,
  const struct rep_object_desc * desc,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);

}


void
rep_object_destroy(
  const uint32_t * object_ids,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(count);

}