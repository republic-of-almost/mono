#include <rep/rep_api.h>
#include <roa_lib/assert.h>
#include <roa_graph/roa_graph.h>
#include <data/engine_data.h>


void
rep_object_create(
  uint32_t * out_object_ids,
  const struct rep_object_desc * desc,
  unsigned count)
{
  ROA_ASSERT(out_object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);

	/* create objects */
	if(out_object_ids && desc && count)
	{
		unsigned i;

		/* create node in graph */
		for(i = 0; i < count; ++i)
		{
			out_object_ids[i] = roa_graph_node_create(
				rep_data_graph());
		}

		/* set parents if any */
		for(i = 0; i < count; ++i)
		{
			if(desc[i].object_parent)
			{
				roa_graph_node_set_parent(
					rep_data_graph(),
					out_object_ids[i],
					desc[i].object_parent);
			}
		}
	}
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

	if(object_ids && desc && count)
	{
		unsigned i;

		/* update parent ids */
		for(i = 0; i < count; ++i)
		{
			roa_graph_node_set_parent(
				rep_data_graph(),
				object_ids[i],
				desc[i].object_parent);
		}
	}
}


void
rep_object_destroy(
  const uint32_t * object_ids,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(count);

	if(object_ids && count)
	{
		unsigned i;

		for(i = 0; i < count; ++i)
		{
			roa_graph_node_remove(
				rep_data_graph(),
				object_ids[i]);
		}
	}
}
