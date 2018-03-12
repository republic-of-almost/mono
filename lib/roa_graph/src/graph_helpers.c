#include <graph_helpers.h>
#include <roa_lib/array.h>
#include <graph_data.h>


unsigned
roa_internal_object_index(uint32_t find, uint32_t *ids, unsigned count)
{
  unsigned index = 0;

  for (index = 0; index < count; ++index)
  {
    if (ids[index] == find)
    {
      return index;
    }
  }

  return index;
}


ROA_BOOL
key_search(uint32_t find, const uint32_t *ids, uint32_t count, uint32_t *index)
{
	uint32_t i;

  for (i = 0; i < count; ++i)
  {
    if (ids[i] == find)
    {
      if (index)
      {
        *index = i;
      }

      return ROA_TRUE;
    }
  }

  return ROA_FALSE;
}
