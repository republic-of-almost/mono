#include <rep/rep_api.h>
#include <roa_lib/assert.h>
#include <roa_graph/roa_graph.h>
#include <roa_renderer/roa_renderer.h>
#include <data/engine_data.h>
#include <data/config.h>


void
rep_mesh_renderable_set(
  uint32_t * object_ids,
  struct rep_mesh_renderable_desc * desc,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);

	if(object_ids && desc && count)
	{
    roa_renderer_ctx_t renderer = rep_data_renderer();
    roa_graph_ctx_t graph = rep_data_graph();
    unsigned i;

    for (i = 0; i < count; ++i)
    {
      uint32_t obj_id = object_ids[i];
      roa_graph_node_register_type(graph, obj_id, REP_DATA_TYPEID_RENDERER_MESH);

      /* get the local transform */
      struct roa_transform transform;
      roa_graph_node_get_transform(graph, obj_id, &transform, ROA_TRUE);

      /* convert world transform */
      roa_mat4 world_transform;
      roa_transform_to_mat4(&transform, &world_transform);

      /* set in the renderer */
      struct roa_renderer_mesh_renderable renderable;
      roa_mat4_export(&world_transform, renderable.world_transform);

      ROA_BOOL set = roa_renderer_mesh_renderable_set(renderer, &renderable, obj_id);
      ROA_ASSERT(set);
    }
	}
}


void
rep_mesh_renderable_get(
  const uint32_t * object_ids,
  struct rep_mesh_renderable_desc * desc,
  unsigned count)
{
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);

	if(object_ids && desc && count)
	{
    ROA_ASSERT(ROA_FALSE);
	}
}
