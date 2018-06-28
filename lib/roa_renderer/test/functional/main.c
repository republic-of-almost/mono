#include <roa_renderer/roa_renderer.h>
#include <roa_ctx/roa_ctx.h>
#include <roa_math/math.h>
#include <roa_lib/dir.h>
#include <scratch/geometry.h>
#include <scratch/textures.h>
#include <gltf/gltf.h>
#include <stb/stb_image.h>
#include <stdio.h>
#include <string.h>


/* --------------------------------------------------- [ Func Test Ctx's ] -- */


roa_ctx_t hw_ctx = ROA_NULL;
roa_renderer_ctx_t renderer_ctx = ROA_NULL;


/* ------------------------------------------- [ Func Test Renderer Data ] -- */


uint32_t object_id_counter = 0;
unsigned renderable_count = 0;

float spin = 0.f;
float pitch = 0.f;
float cam_radius = 50.f;


int
gl_type_to_roa_renderer(int i)
{
  switch(i)
  {
    case(GLTF_COMPONENT_TYPE_BYTE):           return ROA_RENDERER_TYPE_BYTE;
    case(GLTF_COMPONENT_TYPE_UNSIGNED_BYTE):  return ROA_RENDERER_TYPE_UBYTE;
    case(GLTF_COMPONENT_TYPE_SHORT):          return ROA_RENDERER_TYPE_SHORT;
    case(GLTF_COMPONENT_TYPE_UNSIGNED_SHORT): return ROA_RENDERER_TYPE_USHORT;
    case(GLTF_COMPONENT_TYPE_UNSIGNED_INT):   return ROA_RENDERER_TYPE_UINT;
    case(GLTF_COMPONENT_TYPE_FLOAT):          return ROA_RENDERER_TYPE_FLOAT;
  }

  return ROA_RENDERER_TYPE_UINT;
}


/* --------------------------------------------------------- [ Func Test ] -- */


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
    char import_file[2048];
    ROA_MEM_ZERO(import_file);

    strcat(import_file, roa_exe_dir());
    //strcat(import_file, "assets/renderer_test/cube.gltf");
    //strcat(import_file, "assets/renderer_test/plane_trainer.gltf");
    strcat(import_file, "assets/renderer_test/column_room.gltf");

    roa_renderer_load(renderer_ctx, import_file);
  }

  /* setup texture */
  {
    struct roa_renderer_texture_resource texture_rsrc;
    ROA_MEM_ZERO(texture_rsrc);
    texture_rsrc.name = "TestTexture";

    unsigned bytes = 0;
    unsigned char *raw_png = texture_png_data_blender_1(&bytes);

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
      camera.far_plane      = 400.f;
      camera.field_of_view  = ROA_QUART_TAU * 0.125f;
      camera.position[2]    = +3.f;

      uint32_t camera_id = ++object_id_counter;

      roa_renderer_camera_set(renderer_ctx, &camera, camera_id);
    }

    /* renderable */
    {
        int i;
        renderable_count = 0;
        roa_renderer_mesh_list(renderer_ctx, ROA_NULL, &renderable_count);

        for(i = 0; i < renderable_count; ++i) {
                uint32_t obj_id = ++object_id_counter;

                roa_renderer_mesh_renderable_create(renderer_ctx, obj_id);
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
      cam_radius += (ms_desc.y_scroll * 2.2f);

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
      rot_pos = roa_float3_scale(rot_pos, cam_radius);

      struct roa_renderer_camera camera;
      ROA_MEM_ZERO(camera);
      roa_renderer_camera_get(renderer_ctx, &camera, 1);

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
        uint32_t obj_id = i + 2;

        if(renderable_count > 1)
        {
          float x = roa_float_sin(((float)i * increment)) * radius;
          float y = 1.8f + roa_float_sin((i + increment + time) * 0.25f) * 1.7f;
          float z = roa_float_cos(((float)i * increment)) * radius;

          roa_transform transform;
          transform.position = roa_float3_set_with_values(x, y, z);
          transform.rotation = roa_quaternion_default();
          transform.scale    = roa_float3_set_with_values(1, y, 1);

          roa_renderer_mesh_renderable_transform_set(
            renderer_ctx,
            obj_id,
            (float*)&transform.position,
            (float*)&transform.scale,
            (float*)&transform.rotation);
        }
        else
        {
          float width = 4.f;

          roa_transform transform;
          transform.position = roa_float3_set_with_values(0, 0, 0);
          transform.rotation = roa_quaternion_default();
          transform.scale    = roa_float3_set_with_values(width, width, width);

          roa_renderer_mesh_renderable_transform_set(
            renderer_ctx,
            obj_id,
            (float*)&transform.position,
            (float*)&transform.scale,
            (float*)&transform.rotation);

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
        roa_renderer_task_run(renderer_ctx, &tasks[i]);
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
