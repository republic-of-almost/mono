#include <roa_renderer/roa_renderer.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_math/math.h>
#include <stdio.h>


/* -------------------------------------------------- [ Func Test Ctx's ] -- */


roa_ctx_t hw_ctx = ROA_NULL;
roa_renderer_ctx_t renderer_ctx = ROA_NULL;


/* ------------------------------------------ [ Func Test Renderer Data ] -- */


uint32_t object_id_counter = 0;
unsigned renderable_count = 24;
uint64_t cube_mesh = 0;


/* -------------------------------------------------------- [ Func Test ] -- */


int
main()
{
  /* setup systems */
  {
    roa_ctx_create(&hw_ctx);
	  roa_renderer_ctx_create(&renderer_ctx, ROA_NULL);

    struct roa_ctx_window_desc win_desc;
    roa_ctx_get_window_desc(hw_ctx, &win_desc);

    roa_renderer_set_device_resolution(renderer_ctx, win_desc.width, win_desc.height);
  }
	
	/* setup resources */
	{
		struct roa_renderer_mesh_resource mesh_rsrc;
		mesh_rsrc.name = "cube_mesh";

		cube_mesh = roa_renderer_mesh_resource_add(renderer_ctx, &mesh_rsrc);
	}

  /* setup objects */
  {
    ROA_MEM_ZERO(object_id_counter);

    /* camera */
    {
      struct roa_renderer_camera camera;
      ROA_MEM_ZERO(camera);
      camera.near_plane     = 0.1f;
      camera.far_plane      = 1000.f;
      camera.field_of_view  = 3.1421f * 0.125f;
      camera.position[2]    = +3.f;
      
      uint32_t camera_id = ++object_id_counter;

      roa_renderer_camera_set(renderer_ctx, &camera, camera_id);
    }

    /* renderable */
    {
      unsigned i;

      for(i = 0; i < renderable_count; ++i)
      {
        struct roa_renderer_renderable renderable;
        ROA_MEM_ZERO(renderable);

        uint32_t obj_id = ++object_id_counter;

        roa_renderer_renderable_set(renderer_ctx, &renderable, obj_id);
      }
    }
  }

  /* process the scene */
  while (roa_ctx_new_frame(hw_ctx))
  {
    static float time = 0.f;
    time += 0.1f;

    /* set camera */
    {
      struct roa_renderer_camera camera;
      roa_renderer_camera_get(renderer_ctx, &camera, 1);
     
			float radius = 10.f;
			float spin_time = time * 0.025f;

      float x = roa_float_sin(spin_time) * radius;
      float y = radius - (radius / ROA_G_RATIO);
      float z = roa_float_cos(spin_time) * radius;

      roa_float3 from = roa_float3_set_with_values(x, y, z);
      roa_float3 at = roa_float3_fill_with_value(0.f);
      roa_float3 up = roa_float3_set_with_values(0.f, 1.f, 0.f);

			roa_float3_export(from, camera.position);
			roa_float3_export(at, camera.lookat);
			roa_float3_export(up, camera.up);

			roa_renderer_camera_set(renderer_ctx, &camera, 1);
    }

    /* set renderables */
    {
      float increment = ROA_TAU / (float)renderable_count;
      float radius = 5.f;

      unsigned i;

      for (i = 1; i < renderable_count + 1; ++i)
      {
        struct roa_renderer_renderable renderable;
        roa_renderer_renderable_get(renderer_ctx, &renderable, i);

        float x = roa_float_sin(((float)i * increment)) * radius;
        float y = 1.8f + roa_float_sin((i + increment + time) * 0.25f) * 1.7f;
        float z = roa_float_cos(((float)i * increment)) * radius;

        roa_transform transform;
        transform.position = roa_float3_set_with_values(x, y * 0.5f, z);
        transform.rotation = roa_quaternion_default();
        transform.scale = roa_float3_set_with_values(1, y, 1);

        roa_mat4 world;
        roa_transform_to_mat4(&transform, &world);

        ROA_MEM_CPY(renderable.world_transform, world.data);

        roa_renderer_renderable_set(renderer_ctx, &renderable, i);
      }
    }

    /* pump renderer events */
    while(1)
    {
      struct roa_renderer_task *tasks = ROA_NULL;
      unsigned task_count = 0;

      task_count = roa_renderer_task_pump(renderer_ctx, &tasks);

      if (task_count == 0)
      {
        /* no more tasks */
        break;
      }

      unsigned i;
      for(i = 0; i < task_count; ++i)
      {
        roa_renderer_task_run(&tasks[i]);
      }
    }

    roa_renderer_ctx_execute(renderer_ctx);
  }


  /* time to die */
  {
	  roa_renderer_ctx_destroy(&renderer_ctx);
    roa_ctx_destroy(&hw_ctx);
  }

	return 0;
}
