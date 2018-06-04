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
    char import_file[2048];
    ROA_MEM_ZERO(import_file);

    strcat(import_file, roa_exe_dir());
    strcat(import_file, "assets/cube.gltf");

    struct gltf_import gltf;
    ROA_MEM_ZERO(gltf);

    gltf_import(import_file, &gltf);

		struct roa_renderer_mesh_resource mesh_rsrc;
    ROA_MEM_ZERO(mesh_rsrc);
		mesh_rsrc.name = gltf.meshes[0].name;

    int pos = gltf.meshes[0].primitives[0].attributes.POSITION;
    int pos_view = gltf.accessors[pos].buffer_view;
    int pos_buffer = gltf.buffer_views[pos_view].buffer;
    int pos_offset = gltf.buffer_views[pos_view].byte_offset;

    mesh_rsrc.position_vec3_array = (float*)&gltf.buffers[pos_buffer].uri_data[pos_offset];

    int nor = gltf.meshes[0].primitives[0].attributes.NORMAL;
    int nor_view = gltf.accessors[nor].buffer_view;
    int nor_buffer = gltf.buffer_views[nor_view].buffer;
    int nor_offset = gltf.buffer_views[nor_view].byte_offset;

    mesh_rsrc.normal_vec3_array = (float*)&gltf.buffers[nor_buffer].uri_data[nor_offset];

    int texc = gltf.meshes[0].primitives[0].attributes.TEXCOORD_0;
    int texc_view = gltf.accessors[texc].buffer_view;
    int texc_buffer = gltf.buffer_views[texc_view].buffer;
    int texc_offset = gltf.buffer_views[texc_view].byte_offset;

    const unsigned char *buffer = gltf.buffers[texc_buffer].uri_data;
    const unsigned char *texc_rdata = &buffer[texc_offset];
    const float *texc_fdata = (float*)texc_rdata;

    float a = texc_fdata[0];

    mesh_rsrc.texture_coord_vec2_array = texc_fdata;
;

    mesh_rsrc.vertex_count = gltf.accessors[pos].count;

    int index = gltf.meshes[0].primitives[0].indices;
    int index_view = gltf.accessors[index].buffer_view;
    int index_buffer = gltf.buffer_views[index_view].buffer;
    int index_offset = gltf.buffer_views[index_view].byte_offset;

    mesh_rsrc.index_array = (unsigned int*)&gltf.buffers[index_buffer].uri_data[index_offset];
    mesh_rsrc.index_count = gltf.accessors[index_view].count;

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
      camera.field_of_view  = ROA_QUART_TAU * 0.125f;
      camera.position[2]    = +3.f;

      uint32_t camera_id = ++object_id_counter;

      roa_renderer_camera_set(renderer_ctx, &camera, camera_id);
    }

    /* renderable */
    {
      unsigned i;

      for(i = 0; i < renderable_count; ++i)
      {
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
      rot_pos = roa_float3_scale(rot_pos, 60.f);

      struct roa_renderer_camera camera;
      ROA_MEM_ZERO(camera);
      roa_renderer_camera_get(renderer_ctx, &camera, 1);

			float radius = 150.f;
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

          float export[16];
          roa_transform_export_mat4(&transform, export);

          roa_renderer_mesh_renderable_transform_set(renderer_ctx, obj_id, export);
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
          transform.scale    = roa_float3_set_with_values(width, width, width);

          float export[16];
          roa_transform_export_mat4(&transform, export);

          roa_renderer_mesh_renderable_transform_set(renderer_ctx, obj_id, export);
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
