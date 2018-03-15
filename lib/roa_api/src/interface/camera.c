#include <rep/rep_api.h>
#include <roa_lib/assert.h>
#include <roa_renderer/roa_renderer.h>
#include <data/engine_data.h>
#include <roa_graph/roa_graph.h>
#include <roa_math/math.h>


void
rep_camera_set(
  uint32_t * object_ids,
  struct rep_camera_desc * desc,
  unsigned count)
{
  /* param check */
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);

  /* set cameras */
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

      /* set in the renderer */
      struct roa_renderer_camera cam_desc;
      roa_float3_export(cam_transform.position, cam_desc.position);
      cam_desc.field_of_view = desc->fov;

      ROA_BOOL set = roa_renderer_camera_set(renderer, &cam_desc, object_ids[i]);
      ROA_ASSERT(set);
    }
  }
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

  ROA_ASSERT(0); /* no impl */
}