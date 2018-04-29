#include <rep/rep_api.h>
#include <roa_lib/assert.h>
#include <roa_renderer/roa_renderer.h>
#include <data/engine_data.h>
#include <data/config.h>
#include <roa_graph/roa_graph.h>
#include <roa_math/math.h>
#include <stdlib.h>


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
      /* set graph data type */
      roa_graph_node_register_type(graph, object_ids[i], REP_DATA_TYPEID_CAMERA);

      /* get the local transform */
      struct roa_transform transform;
      roa_graph_node_get_transform(graph, object_ids[i], &transform, ROA_FALSE);

      /* set in the renderer */
      struct roa_renderer_camera cam_desc;
      cam_desc.field_of_view = desc->fov;
      cam_desc.near_plane = 0.1f;
      cam_desc.far_plane = 1000.f;

      /* set transform */
      roa_float3 fwd = roa_transform_local_fwd(&transform);
      roa_float3 at = roa_float3_add(transform.position, fwd);
      roa_float3 up = roa_transform_local_up(&transform);

      memcpy(cam_desc.position, &transform.position, sizeof(cam_desc.position));
      memcpy(cam_desc.lookat, &at, sizeof(cam_desc.lookat));
      memcpy(cam_desc.up, &up, sizeof(cam_desc.up));

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