#include <roa_renderer/roa_renderer.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_math/math.h>
#include <scratch/geometry.h>
#include <scratch/textures.h>
#include <stb/stb_image.h>
#include <stdio.h>


/* -------------------------------------------------- [ Func Test Ctx's ] -- */


roa_ctx_t hw_ctx = ROA_NULL;
roa_renderer_ctx_t renderer_ctx = ROA_NULL;


/* ------------------------------------------ [ Func Test Renderer Data ] -- */


uint32_t object_id_counter = 0;
unsigned renderable_count = 1;
uint64_t cube_mesh = 0;

float spin = 0.f;
float pitch = 0.f;


/* -------------------------------------------------------- [ Func Test ] -- */


int
main()
{
  /* setup systems */
  {
    roa_ctx_create(&hw_ctx);
	  roa_renderer_ctx_create(&renderer_ctx, ROA_NULL);

    struct roa_ctx_window_desc win_desc;
    ROA_MEM_ZERO(win_desc);
    win_desc.width = 1280;
    win_desc.height = 720;
    win_desc.title = "ROA Renderer Func Test";
    win_desc.capture_mouse = ROA_TRUE;

    roa_ctx_set_window_desc(hw_ctx, &win_desc);
  }

	/* setup mesh resources */
	{
    float pos[1024];
    float uv[1024];
    float norm[1024];
    int vert_count = 0;

    {
      geom_vert_desc vert_desc[] = {
        GEOM_VERT_POSITION3,
      };

      geometry_generate_cube(
        ROA_ARR_DATA(vert_desc),
        ROA_ARR_COUNT(vert_desc),
        1,
        1,
        1,
        ROA_ARR_DATA(pos),
        &vert_count);
    }

    {
      geom_vert_desc vert_desc[] = {
        GEOM_NORMAL,
      };

      geometry_generate_cube(
        ROA_ARR_DATA(vert_desc),
        ROA_ARR_COUNT(vert_desc),
        1,
        1,
        1,
        ROA_ARR_DATA(norm),
        &vert_count);
    }

    {
      geom_vert_desc vert_desc[] = {
        GEOM_UV,
      };

      geometry_generate_cube(
        ROA_ARR_DATA(vert_desc),
        ROA_ARR_COUNT(vert_desc),
        1,
        1,
        1,
        ROA_ARR_DATA(uv),
        &vert_count);
    }

		struct roa_renderer_mesh_resource mesh_rsrc;
    ROA_MEM_ZERO(mesh_rsrc);
		mesh_rsrc.name = "CubeMesh";
    mesh_rsrc.position_vec3_array = ROA_ARR_DATA(pos);
    mesh_rsrc.normal_vec3_array = ROA_ARR_DATA(norm);
    mesh_rsrc.texture_coord_vec2_array = ROA_ARR_DATA(uv);
    mesh_rsrc.vertex_count = vert_count;

		cube_mesh = roa_renderer_mesh_resource_add(renderer_ctx, &mesh_rsrc);
	}

  /* setup texture */
  {
    struct roa_renderer_texture_resource texture_rsrc;
    ROA_MEM_ZERO(texture_rsrc);
    texture_rsrc.name = "TestTexture";
   
    unsigned bytes = 0;
    unsigned *raw_png = texture_png_data_blender_1(&bytes);

    stbi_set_flip_vertically_on_load(1);
    texture_rsrc.data = stbi_load_from_memory(
      raw_png, bytes, &texture_rsrc.width, &texture_rsrc.height, &texture_rsrc.components, 3);

    roa_renderer_texture_resource_add(renderer_ctx, &texture_rsrc);
  }

  /* setup objects */
  {
    ROA_MEM_ZERO(object_id_counter);

    /* camera */
    {
      struct roa_renderer_camera camera;
      ROA_MEM_ZERO(camera);
      camera.near_plane     = 0.1f;
      camera.far_plane      = 100.f;
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
        struct roa_renderer_mesh_renderable renderable;
        ROA_MEM_ZERO(renderable);

        uint32_t obj_id = ++object_id_counter;

        roa_renderer_mesh_renderable_set(renderer_ctx, &renderable, obj_id);
      }
    }
  }

  /* process the scene */
  while (roa_ctx_new_frame(hw_ctx))
  {
    /* quit if escape pressed */
    {
      struct roa_ctx_keyboard_desc kb_desc;
      roa_ctx_keyboard_get_desc(hw_ctx, &kb_desc);

      if(kb_desc.kb_keys[ROA_KB_ESC])
      {
        break;
      }
    }

    static float time = 0.f;
    time += 0.1f;

    /* set resolution */
    {
      struct roa_ctx_window_desc win_desc;
      ROA_MEM_ZERO(win_desc);

      roa_ctx_get_window_desc(hw_ctx, &win_desc);

      struct roa_renderer_device_viewport vp;
      ROA_MEM_ZERO(vp);

      vp.width = win_desc.width;
      vp.height = win_desc.height;

      roa_renderer_set_device_viewport(renderer_ctx, &vp);
    }

    /* set camera */
    {
      struct roa_ctx_mouse_desc ms_desc;
      ROA_MEM_ZERO(ms_desc);
      roa_ctx_mouse_get_desc(hw_ctx, &ms_desc);

      spin += (ms_desc.x_delta * 0.001f);
      pitch += (ms_desc.y_delta * 0.001f);

      pitch = roa_float_clamp(pitch, -ROA_QUART_TAU, +ROA_QUART_TAU);

      roa_quaternion yaw_rot = roa_quaternion_from_axis_angle(
        roa_transform_world_up(),
        spin);

      roa_quaternion pitch_rot = roa_quaternion_from_axis_angle(
        roa_transform_world_fwd(),
        pitch);

      roa_quaternion final_rot = roa_quaternion_multiply(yaw_rot, pitch_rot);
      roa_float3 rot_pos = roa_quaternion_rotate_vector(
        final_rot,
        roa_transform_world_left());
      rot_pos = roa_float3_scale(rot_pos, 30.f);
      
      struct roa_renderer_camera camera;
      ROA_MEM_ZERO(camera);
      roa_renderer_camera_get(renderer_ctx, &camera, 1);

			float radius = 20.f;
			float spin_time = time * 0.025f;

      float x = roa_float_sin(spin) * radius;
      float y = radius - (radius / ROA_G_RATIO);
      float z = roa_float_cos(spin) * radius; 

      roa_float3 from = roa_float3_set_with_values(x, y, z);
      roa_float3 at   = roa_float3_fill_with_value(0.f);
      roa_float3 up   = roa_quaternion_rotate_vector(
        final_rot, roa_transform_world_up());

			roa_float3_export(rot_pos, camera.position);
			roa_float3_export(at, camera.lookat);
			roa_float3_export(up, camera.up);

			roa_renderer_camera_set(renderer_ctx, &camera, 1);
    }

    /* set renderables */
    {
      float increment = ROA_TAU / (float)renderable_count;
      float radius = 9.f;

      unsigned i;

      for (i = 0; i < renderable_count; ++i)
      {
        uint32_t obj_id = i + 1;

        struct roa_renderer_mesh_renderable renderable;
        ROA_MEM_ZERO(renderable);
        roa_renderer_mesh_renderable_get(renderer_ctx, &renderable, obj_id);

        if(renderable_count > 1)
        {
          float x = roa_float_sin(((float)i * increment)) * radius;
          float y = 1.8f + roa_float_sin((i + increment + time) * 0.25f) * 1.7f;
          float z = roa_float_cos(((float)i * increment)) * radius;

          roa_transform transform;
          transform.position = roa_float3_set_with_values(x, y, z);
          transform.rotation = roa_quaternion_default();
          transform.scale    = roa_float3_set_with_values(1, y, 1);

          roa_transform_export_mat4(&transform, renderable.world_transform);

          roa_renderer_mesh_renderable_set(renderer_ctx, &renderable, obj_id);
        }
        else
        {
          float width = 4.f;
          float height = width * ROA_G_RATIO;

//          float width = 10.f;
//          float height = 0.1f;

          roa_transform transform;
          transform.position = roa_float3_set_with_values(0, 0, 0);
          transform.rotation = roa_quaternion_default();
          transform.scale    = roa_float3_set_with_values(width, height, width);

          roa_transform_export_mat4(&transform, renderable.world_transform);

          roa_renderer_mesh_renderable_set(renderer_ctx, &renderable, obj_id);
        }
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
