#include <roa_ctx/roa_ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_math/math.h>
#include <volt/volt.h>
#include <roa_lib/time.h>
#include <GL/gl3w.h>
#include <scratch/geometry.h>


/* ---------------------------------------------------------- [ Systems ] -- */


roa_ctx_t                 hw_ctx;     /* windowing */
volt_ctx_t                volt_ctx;   /* graphics api */


/* ------------------------------------------------------------ [ Scene ] -- */


int width = 1200;
int height = 720;

unsigned char *texture_data = ROA_NULL;

struct g_buffer_data
{
	volt_program_t 			program;
	volt_framebuffer_t 	fbo;
	volt_texture_t 			fbo_color_outputs[5];
	volt_texture_t 			fbo_depth;
	volt_input_t 				input;
	volt_rasterizer_t 	rasterizer;

} g_buffer;


struct scene_data
{
	volt_vbo_t vbo;
  roa_transform box_transform[5];
  roa_mat4 box_world[5];
  roa_mat4 box_wvp[5];

  volt_uniform_t box_world_uni[5];
  volt_uniform_t box_wvp_uni[5];

  roa_mat4 view_mat;
  roa_mat4 proj_mat;

  roa_float3 cam_position;
  float cam_pitch;
  float cam_yaw;
} scene;


/* ------------------------------------------------------- [ Applicaton ] -- */


int
main(int argc, char **argv)
{
  /* command line args */
  ROA_UNUSED(argc);
  ROA_UNUSED(argv);

  /* -------------------------------------------------------- [ Systems ] -- */
  {
    struct roa_ctx_window_desc win_desc;
    win_desc.width = width;
    win_desc.height = height;
    win_desc.title = "Decals";

    roa_ctx_create(&hw_ctx);
    roa_ctx_set_window_desc(hw_ctx, &win_desc);

    volt_ctx_create(&volt_ctx);
  }
  
  /* ---------------------------------------------------------- [ Scene ] -- */
  {
    /* load vbo */
    {
      geom_vert_desc desc[3];
      desc[0] = GEOM_VERT_POSITION;
      desc[1] = GEOM_UV;
      desc[2] = GEOM_NORMAL;

      unsigned vert_count;

      geometry_generate_cube(desc, 3, 1, 1, 1, ROA_NULL, &vert_count);

      float *data = malloc(sizeof(float) * vert_count);

      geometry_generate_cube(desc, 3, 1, 1, 1, data, &vert_count);

      struct volt_vbo_desc vbo_desc;
      vbo_desc.data   = data;
      vbo_desc.count  = vert_count;

      volt_vertex_buffer_create(volt_ctx, &scene.vbo, &vbo_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* box positions */
    {
      roa_transform_init(&scene.box_transform[0]);
      scene.box_transform[0].position = roa_float3_set_with_values(0.f, 0.f, 5.f);
      
      roa_transform_init(&scene.box_transform[1]);
      scene.box_transform[1].position = roa_float3_set_with_values(6.f, 1.f, 10.f);

      roa_transform_init(&scene.box_transform[2]);
      scene.box_transform[2].position = roa_float3_set_with_values(-5.f, -1.f, 12.f);

      roa_transform_init(&scene.box_transform[3]);
      scene.box_transform[3].position = roa_float3_set_with_values(4.f, 4.f, 15.f);

      roa_transform_init(&scene.box_transform[4]);
      scene.box_transform[4].position = roa_float3_set_with_values(-4.f, 2.f, 20.f);
    }

    /* uniforms */
    {
      int i;
      int count = ROA_ARR_COUNT(scene.box_transform);

      for(i = 0; i < count; ++i)
      {
        struct volt_uniform_desc wvp_uni_desc;
        wvp_uni_desc.data_type = VOLT_DATA_MAT4;
        wvp_uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &scene.box_wvp_uni[i], &wvp_uni_desc);

        struct volt_uniform_desc world_uni_desc;
        world_uni_desc.data_type = VOLT_DATA_MAT4;
        world_uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &scene.box_world_uni[i], &world_uni_desc);
      }

      volt_ctx_execute(volt_ctx);
    }

    /* projection camera */
    {
      float aspect = (float)width / (float)height;
      float fov = ROA_TAU * 0.125f;

      roa_mat4_projection(&scene.proj_mat, fov, 0.1, 100, aspect);

      scene.cam_pitch    = 0.f;
      scene.cam_yaw      = 0.f;
      scene.cam_position = roa_float3_one();
    }
  }

  /* ------------------------------------------------------- [ G-Buffer ] -- */
	{
		/* color outputs */
		{
      /* texture names */
      const char *g_buffer_texture_names[ROA_ARR_COUNT(g_buffer.fbo_color_outputs)] = {
        "GBuffer:Positions",
        "GBuffer:Diffuse",
        "GBuffer:Normals",
        "GBuffer:TexCoords",
        "GBuffer:Final",
      };

			int i;
			for(i = 0; i < ROA_ARR_COUNT(g_buffer.fbo_color_outputs); ++i)
			{
				struct volt_texture_desc tex_desc;
				tex_desc.width      = width;
				tex_desc.height     = height;
				tex_desc.dimentions = VOLT_TEXTURE_2D;
				tex_desc.sampling   = VOLT_SAMPLING_BILINEAR;
				tex_desc.format     = VOLT_PIXEL_FORMAT_RGBA32F;
        tex_desc.mip_maps   = VOLT_FALSE;
        tex_desc.data       = ROA_NULL;
        tex_desc.access     = VOLT_STATIC;
				tex_desc.name       = g_buffer_texture_names[i];

				volt_texture_create(volt_ctx, &g_buffer.fbo_color_outputs[i], &tex_desc);
        volt_ctx_execute(volt_ctx);
			}
		}

		/* depth buffer */
		{
			struct volt_texture_desc tex_desc;
			tex_desc.width      = width;
			tex_desc.height     = height;
			tex_desc.dimentions = VOLT_TEXTURE_2D;
			tex_desc.sampling   = VOLT_SAMPLING_BILINEAR;
			tex_desc.format     = VOLT_PIXEL_FORMAT_DEPTH32F;
      tex_desc.data       = ROA_NULL;
      tex_desc.access     = VOLT_STATIC;
      tex_desc.mip_maps   = VOLT_FALSE;
			tex_desc.name       = "GBuffer:Depth";

			volt_texture_create(volt_ctx, &g_buffer.fbo_depth, &tex_desc);
      volt_ctx_execute(volt_ctx);
		}

		/* create fbo */
		{
			struct volt_framebuffer_desc fbo_desc;
			fbo_desc.attachments      = g_buffer.fbo_color_outputs;
			fbo_desc.attachment_count = ROA_ARR_COUNT(g_buffer.fbo_color_outputs);
			fbo_desc.depth            = g_buffer.fbo_depth;

			volt_framebuffer_create(volt_ctx, &g_buffer.fbo, &fbo_desc);
      volt_ctx_execute(volt_ctx);
		}

    /* shader */
    {
      const char vs[] = ""
        "#version 330\n"

        "layout(location = 0) in vec3 Position;\n"
        "layout (location = 1) in vec2 TexCoord; \n"
        "layout(location = 2) in vec3 Normal;\n"

        "uniform mat4 gWVP;\n"
        "uniform mat4 gWorld;\n"

        "out vec2 TexCoord0;\n"
        "out vec3 Normal0;\n"
        "out vec3 WorldPos0;\n"

        "void main()\n"
        "{\n"
        " gl_Position = gWVP * vec4(Position, 1.0);\n"
        " TexCoord0 = TexCoord;\n"
        " Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;\n"
        " WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;\n"
        "}\n";

      const char fs[] = ""
        "#version 330\n"

        "in vec2 TexCoord0;   \n"
        "in vec3 Normal0;   \n"
        "in vec3 WorldPos0;  \n"

        "layout (location = 0) out vec3 WorldPosOut;   \n"
        "layout (location = 1) out vec3 DiffuseOut;  \n"
        "layout (location = 2) out vec3 NormalOut;   \n"
        "layout (location = 3) out vec3 TexCoordOut; \n"

        "uniform sampler2D gColorMap;   \n"

        "void main()  \n"
        "{    \n"
        " WorldPosOut     = WorldPos0;    \n"
        " DiffuseOut      = vec3(1, 0, 1); /*texture(gColorMap, TexCoord0).xyz;*/\n"
        " NormalOut       = normalize(Normal0);    \n"
        " TexCoordOut     = vec3(TexCoord0, 0.0);  \n"
        "}\n";

      volt_shader_stage stages[2] = {
        VOLT_SHD_VERTEX,
        VOLT_SHD_FRAGMENT,
      };

      const char *stages_src[2] = {
        vs,
        fs,
      };

      struct volt_program_desc prog_desc;
      prog_desc.shader_stages_src   = stages_src;
      prog_desc.shader_stages_type  = stages;
      prog_desc.stage_count         = ROA_ARR_COUNT(stages);

      volt_program_create(volt_ctx, &g_buffer.program, &prog_desc);
      volt_ctx_execute(volt_ctx);
    }


    /* input format */
    {
      volt_input_attribute attrs[3] = {
        VOLT_INPUT_FLOAT3, /* positions */
        VOLT_INPUT_FLOAT2, /* tex coords  */
        VOLT_INPUT_FLOAT3, /* normals */
      };

      struct volt_input_desc input_desc;
      input_desc.attributes = ROA_ARR_DATA(attrs);
      input_desc.count      = ROA_ARR_COUNT(attrs);

      /*volt_input_t   input_format;*/
      volt_input_create(volt_ctx, &g_buffer.input, &input_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* rasterizer */
    {
      struct volt_rasterizer_desc raster_desc;
      raster_desc.cull_mode       = VOLT_CULL_FRONT;
      raster_desc.primitive_type  = VOLT_PRIM_TRIANGLES;
      raster_desc.winding_order   = VOLT_WIND_CW;

      volt_rasterizer_create(volt_ctx, &g_buffer.rasterizer, &raster_desc);
      volt_ctx_execute(volt_ctx);
    }
	}

  /* ----------------------------------------------------------- [ Loop ] -- */
  while (roa_ctx_new_frame(hw_ctx))
  {
    /* ------------------------------------------ [ Generate Scene Data ] -- */
    {
      /* view mat */
      {
        float radius = 2.5f;

        struct roa_ctx_mouse_desc ms_desc;
        roa_ctx_mouse_get_desc(hw_ctx, &ms_desc);

        scene.cam_pitch += ms_desc.y_delta * 0.01f;
        scene.cam_yaw += ms_desc.x_delta * 0.01f;

        float x = roa_float_sin(scene.cam_yaw) * radius;
        float y = 2.f;
        float z = roa_float_cos(scene.cam_yaw) * radius;

        roa_float3 up   = roa_float3_set_with_values(0, 1, 0);
        roa_float3 pos  = roa_float3_set_with_values(x, y, z);
        roa_float3 at   = roa_float3_zero();

        roa_mat4_lookat(&scene.view_mat, pos, at, up);
      }

      /* world */
      {
        int count = ROA_ARR_COUNT(scene.box_transform);
        int i;

        roa_mat4 view_proj;
        roa_mat4_multiply(&view_proj, &scene.view_mat, &scene.proj_mat);

        for (i = 0; i < count; ++i)
        {
          roa_transform_to_mat4(&scene.box_transform[i], &scene.box_world[i]);
          roa_mat4_multiply(&scene.box_wvp[i], &scene.box_world[i], &view_proj);

          volt_uniform_update(volt_ctx, scene.box_world_uni[i], (void*)scene.box_world[i].data);
          volt_uniform_update(volt_ctx, scene.box_wvp_uni[i], (void*)scene.box_wvp[i].data);

          volt_ctx_execute(volt_ctx);
        }
      }
    }

    /* ------------------------------------------ [ G Buffer Renderpass ] -- */
    {
      volt_renderpass_t g_buffer_pass;
      volt_renderpass_create(volt_ctx, &g_buffer_pass, "Fill GBuffer", g_buffer.fbo);

      /* setup gbuffer */
      volt_renderpass_bind_program(g_buffer_pass, g_buffer.program);
      volt_renderpass_bind_rasterizer(g_buffer_pass, g_buffer.rasterizer);
      volt_renderpass_bind_input_format(g_buffer_pass, g_buffer.input);
      
      /* render geometry */
      volt_renderpass_bind_vertex_buffer(g_buffer_pass, scene.vbo);
      volt_renderpass_set_viewport(g_buffer_pass, 0, 0, width, height);

      int i;
      int count = ROA_ARR_COUNT(scene.box_world);

      for (i = 0; i < count; ++i)
      {
        volt_renderpass_bind_uniform(g_buffer_pass, scene.box_wvp_uni[i], "gWVP");
        volt_renderpass_bind_uniform(g_buffer_pass, scene.box_world_uni[i], "gWorld");
        volt_renderpass_draw(g_buffer_pass);
      }

      volt_renderpass_commit(volt_ctx, &g_buffer_pass);
    }

    volt_ctx_execute(volt_ctx);

  } /* while loop */

  /* destroy systems */
  {
    volt_ctx_destroy(&volt_ctx);
    roa_ctx_destroy(&hw_ctx);
  }

  return 0;
}
