#include <rep/rep_api.h>
#include <roa_lib/assert.h>
#include <roa_graph/roa_graph.h>
#include <data/engine_data.h>
#include <roa_renderer/roa_renderer.h>
#include <data/config.h>
#include <stdlib.h>


void
rep_transform_set(
  uint32_t * object_ids,
  struct rep_transform_desc *desc,
  unsigned count)
{
  /* param check */
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);

  /* set transforms */
	if(object_ids && desc && count)
	{
    roa_graph_ctx_t graph = rep_data_graph();
    roa_renderer_ctx_t renderer = rep_data_renderer();

    int i;

    for(i = 0; i < count; ++i)
    {
      uint32_t obj_id = object_ids[i];

      roa_transform transform;
      roa_transform_init(&transform);

      transform.position = roa_float3_import(desc[i].position);
      transform.rotation = roa_quaternion_import(desc[i].rotation);
      transform.scale = roa_float3_import(desc[i].scale);

      roa_graph_node_set_transform(rep_data_graph(), obj_id, &transform);

      /* update any data that is interested */
      {
        uint64_t type_ids;
        roa_graph_node_get_data_type_id(graph, obj_id, &type_ids);

        if (type_ids & REP_DATA_TYPEID_CAMERA)
        {
          roa_float3 fwd = roa_transform_local_fwd(&transform);
          roa_float3 at = roa_float3_add(transform.position, fwd);
          roa_float3 up = roa_transform_local_up(&transform);

          struct roa_renderer_camera cam;
          roa_renderer_camera_get(renderer, &cam, obj_id);

          memcpy(cam.position, desc[i].position, sizeof(cam.position));
          memcpy(cam.lookat, &at, sizeof(cam.lookat));
          memcpy(cam.up, &up, sizeof(cam.up));

          roa_renderer_camera_set(renderer, &cam, obj_id);
        }

        if (type_ids & REP_DATA_TYPEID_RENDERER_MESH)
        {
          struct roa_renderer_mesh_renderable mesh;
          roa_renderer_mesh_renderable_get(renderer, &mesh, obj_id);
          
          roa_transform_export_mat4(&transform, mesh.world_transform);

          roa_renderer_mesh_renderable_set(renderer, &mesh, obj_id);
        }
      }
    }
	}
}


void
rep_transform_get(
  const uint32_t * object_ids,
  struct rep_transform_desc *desc,
  unsigned count)
{
  /* param check */
  ROA_ASSERT(object_ids);
  ROA_ASSERT(desc);
  ROA_ASSERT(count);

  /* get transform */
  if (object_ids && desc && count)
  {
    roa_graph_ctx_t graph = rep_data_graph();
    int i;

    for(i = 0; i < count; ++i)
    {
      uint32_t obj_id = object_ids[i];

      roa_transform transform;
      roa_graph_node_get_transform(graph, obj_id, &transform, ROA_FALSE);

      memcpy(desc[i].position, &transform.position, sizeof(desc[i].position));
      memcpy(desc[i].rotation, &transform.rotation, sizeof(desc[i].rotation));
      memcpy(desc[i].scale, &transform.scale, sizeof(desc[i].scale));
    }
  }
}
