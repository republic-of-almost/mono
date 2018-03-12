#ifndef GRAPH_HELPERS_INCLUDED_894427E3_1666_4989_9124_8F258DD121A6
#define GRAPH_HELPERS_INCLUDED_894427E3_1666_4989_9124_8F258DD121A6


#include <roa_lib/fundamental.h>


unsigned
roa_internal_object_index(uint32_t find, uint32_t *ids, unsigned count);


ROA_BOOL
key_search(uint32_t find, const uint32_t *ids, uint32_t count, uint32_t *index);


#endif /* inc guard */