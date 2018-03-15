#include <rep/rep_api.h>
#include <roa_lib/assert.h>
#include <roa_graph/roa_graph.h>
#include <roa_renderer/roa_renderer.h>


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
      /* get the local transform */
      struct roa_transform cam_transform;
      roa_graph_node_get_transform(graph, object_ids[i], &cam_transform, ROA_TRUE);

      /* convert world transform */
      roa_mat4 world_transform;
      roa_transform_to_mat4(&cam_transform, &world_transform);

      /* set in the renderer */
      struct roa_renderer_renderable renderable;
      roa_mat4_export(&world_transform, renderable.world_transform);

      ROA_BOOL set = roa_renderer_renderable_set(renderer, &renderable, object_ids[i]);
      
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

	}
}
