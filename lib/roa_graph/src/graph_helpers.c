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