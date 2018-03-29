#include <roa_renderer/roa_renderer.h>
#include <roa_ctx/roa_ctx.h>


/* -------------------------------------------------- [ Func Test Ctx's ] -- */


roa_ctx_t hw_ctx = ROA_NULL;
roa_renderer_ctx_t renderer_ctx = ROA_NULL;


/* ------------------------------------------ [ Func Test Renderer Data ] -- */


uint32_t object_id_counter;
uint32_t objects[2];


/* -------------------------------------------------------- [ Func Test ] -- */


int
main()
{
  /* setup systems */
  {
    roa_ctx_create(&hw_ctx);
	  roa_renderer_ctx_create(&renderer_ctx, ROA_NULL);
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
      camera.direction[2]   = -1.f;
      camera.up[1]          = +1.f;
      camera.position[2]    = +3.f;
      
      objects[0] = ++object_id_counter;

      roa_renderer_camera_set(renderer_ctx, &camera, objects[0]);
    }

    /* renderable */
    {
      struct roa_renderer_renderable renderable;
      ROA_MEM_ZERO(renderable);

      objects[1] = ++object_id_counter;

      roa_renderer_renderable_set(renderer_ctx, &renderable, objects[1]);
    }
  }


  /* process the scene */
  while (roa_ctx_new_frame(hw_ctx))
  {
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
