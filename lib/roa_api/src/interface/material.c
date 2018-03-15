#include <rep/rep_api.h>
#include <roa_lib/assert.h>


void
rep_material_find(
  uint64_t * out_material_ids,
  const char ** material_names,
  unsigned material_name_count)
{
  ROA_ASSERT(out_material_ids);
  ROA_ASSERT(material_names);
  ROA_ASSERT(material_name_count);
}