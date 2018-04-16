#include <roa_ctx/roa_ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_math/math.h>
#include <volt/volt.h>
#include <roa_lib/time.h>
#include <scratch/geometry.h>
#include <stdlib.h>
#include <stdio.h>


/* ----------------------------------------------------------- [ Systems ] -- */


roa_ctx_t                 hw_ctx;     /* windowing / input */
volt_ctx_t                volt_ctx;   /* graphics api */


/* ------------------------------------------------------------- [ Scene ] -- */


int width = 1200;
int height = 720;


struct g_buffer_data
{
	volt_program_t 			program;
	volt_framebuffer_t 	fbo;
	volt_texture_t 			fbo_color_outputs[5];
	volt_texture_t 			fbo_depth;
	volt_input_t 				input;
	volt_rasterizer_t 	rasterizer;

	struct volt_pipeline_desc pipeline_desc;
} g_buffer;


struct point_light_data
{
  volt_program_t      program;
  volt_rasterizer_t 	rasterizer;
  volt_input_t 				input;
} point_lights;


struct direction_light_data
{
  volt_program_t    program;
  volt_rasterizer_t rasterizer;
  volt_vbo_t        triangle;
  volt_input_t      input;
  volt_uniform_t    eye_pos_uni;
  volt_uniform_t    wvp_uni;

  volt_sampler_t    u_color;
  volt_sampler_t    u_position;
  volt_sampler_t    u_normal;

  struct volt_pipeline_desc pipeline_desc;
  struct volt_draw_desc     draw_desc;

} dir_lights;


struct scene_data
{
  struct volt_rect2d viewport;

	volt_vbo_t vbo;
  roa_transform box_transform[9];
  roa_mat4 box_world[9];
  roa_mat4 box_wvp[9];
	struct volt_draw_desc draw_desc[9];

  volt_uniform_t box_world_uni;
  volt_uniform_t box_wvp_uni;

  roa_mat4 view_mat;
  roa_mat4 proj_mat;

  roa_float3 cam_position;
  float cam_pitch;
  float cam_yaw;
} scene;


struct fullscreen_data
{
	volt_program_t    program;
	volt_vbo_t        triangle;
	volt_input_t      input;
	volt_rasterizer_t rasterizer;

  volt_sampler_t    u_diffuse;

	struct volt_pipeline_desc pipeline_desc;
	struct volt_draw_desc 		draw_desc;
} fullscreen;


/* -------------------------------------------------------- [ Applicaton ] -- */


void
volt_cb(const char *msg)
{
  printf("%s", msg);
}


int
main(int argc, char **argv)
{
  /* command line args */
  ROA_UNUSED(argc);
  ROA_UNUSED(argv);

  /* --------------------------------------------------------- [ Systems ] -- */
  {
    struct roa_ctx_window_desc win_desc;
    ROA_MEM_ZERO(win_desc);

    win_desc.width = width;
    win_desc.height = height;
    win_desc.title = "Decals";

    roa_ctx_create(&hw_ctx);
    roa_ctx_set_window_desc(hw_ctx, &win_desc);

    volt_ctx_create(&volt_ctx);
    volt_ctx_logging_callback(volt_ctx, volt_cb);
  }

  /* ----------------------------------------------------------- [ Scene ] -- */
  {
    struct volt_rect2d vp = {0,0,width, height};
    scene.viewport = vp;

    /* load vbo */
    {
      geom_vert_desc desc[] = {
        GEOM_VERT_POSITION,
        GEOM_UV,
        GEOM_NORMAL,
      };

      unsigned vert_count;

      geometry_generate_cube(desc, 3, 1, 1, 1, ROA_NULL, &vert_count);

      float *data = malloc(sizeof(float) * vert_count);

      geometry_generate_cube(desc, 3, 1, 1, 1, data, &vert_count);

      struct volt_vbo_desc vbo_desc;
      ROA_MEM_ZERO(vbo_desc);

      vbo_desc.data = data;
      vbo_desc.count = vert_count;

      volt_vertex_buffer_create(volt_ctx, &scene.vbo, &vbo_desc);
      volt_ctx_execute(volt_ctx);
    }

		/* draw desc */
		{
			int count = ROA_ARR_COUNT(scene.draw_desc);
			int i;

			for(i = 0; i < count; ++i)
			{
				ROA_MEM_ZERO(scene.draw_desc[i]);
				scene.draw_desc[i].vbo = scene.vbo;
			}
		}

    /* box positions */
    {
      roa_transform_init(&scene.box_transform[0]);
      scene.box_transform[0].position = roa_float3_set_with_values(+2.f, 0.f, +2.f);

      roa_transform_init(&scene.box_transform[1]);
      scene.box_transform[1].position = roa_float3_set_with_values(+2.f, 0.f, -2.f);

      roa_transform_init(&scene.box_transform[2]);
      scene.box_transform[2].position = roa_float3_set_with_values(-2.f, 0.f, +2.f);

      roa_transform_init(&scene.box_transform[3]);
      scene.box_transform[3].position = roa_float3_set_with_values(-2.f, 0.f, -2.f);


      roa_transform_init(&scene.box_transform[4]);
      scene.box_transform[4].position = roa_float3_set_with_values(+2.f, 3.f, +2.f);

      roa_transform_init(&scene.box_transform[5]);
      scene.box_transform[5].position = roa_float3_set_with_values(+2.f, 3.f, -2.f);

      roa_transform_init(&scene.box_transform[6]);
      scene.box_transform[6].position = roa_float3_set_with_values(-2.f, 3.f, +2.f);

      roa_transform_init(&scene.box_transform[7]);
      scene.box_transform[7].position = roa_float3_set_with_values(-2.f, 3.f, -2.f);

      roa_transform_init(&scene.box_transform[8]);
      scene.box_transform[8].position = roa_float3_set_with_values(0.f, 0.f, 0.f);
    }

    /* uniforms */
    {
      int i;
      int count = ROA_ARR_COUNT(scene.box_transform);

      struct volt_uniform_desc wvp_uni_desc;
			{
	      ROA_MEM_ZERO(wvp_uni_desc);

				wvp_uni_desc.name 		 = "gWVP";
	      wvp_uni_desc.data_type = VOLT_DATA_MAT4;
	      wvp_uni_desc.count 		 = 1;
				wvp_uni_desc.copy_data = VOLT_FALSE;

	      volt_uniform_create(volt_ctx, &scene.box_wvp_uni, &wvp_uni_desc);
			}

      struct volt_uniform_desc world_uni_desc;
			{
	      ROA_MEM_ZERO(world_uni_desc);

				world_uni_desc.name 		 = "gWorld";
	      world_uni_desc.data_type = VOLT_DATA_MAT4;
	      world_uni_desc.count 		 = 1;
				world_uni_desc.copy_data = VOLT_FALSE;

	      volt_uniform_create(volt_ctx, &scene.box_world_uni, &world_uni_desc);
			}

      volt_ctx_execute(volt_ctx);
    }

    /* projection camera */
    {
      float aspect 	= (float)width / (float)height;
      float fov 		= ROA_TAU * 0.125f;

      roa_mat4_projection(&scene.proj_mat, fov, 0.1, 100, aspect);

      scene.cam_pitch 	 = 0.f;
      scene.cam_yaw 		 = 0.f;
      scene.cam_position = roa_float3_one();
    }
  }

	/* ------------------------------------------------------ [ Fullscreen ] -- */
	{
		/* program */
		{
			const char *vert_shd =
				"#version 150 core\n"
				"in vec2 vs_in_pos;\n"
				"in vec2 vs_in_tex_c;\n"
				"out vec2 ps_in_tex_c;\n"
				"void main()\n"
				"{\n"
				" gl_Position = vec4(vs_in_pos, 0.0, 1.0);\n"
				" ps_in_tex_c = vs_in_tex_c;\n"
				"}\n";

			const char *frag_shd =
				"#version 150 core\n"
				"in vec2 ps_in_tex_c;\n"
				"out vec4 out_color;\n"
				"uniform sampler2D diffuse;\n"
				"void main()\n"
				"{\n"
				" out_color = texture(diffuse, ps_in_tex_c);\n"
				"}\n";

			const char *shaders[] = {
				vert_shd,
				frag_shd,
			};

			volt_shader_stage stages[] = {
				VOLT_SHD_VERTEX,
				VOLT_SHD_FRAGMENT,
			};

			struct volt_program_desc screen_program_desc;
      ROA_MEM_ZERO(screen_program_desc);

			screen_program_desc.stage_count         = ROA_ARR_COUNT(stages);
			screen_program_desc.shader_stages_type  = ROA_ARR_DATA(stages);
			screen_program_desc.shader_stages_src   = ROA_ARR_DATA(shaders);

			volt_program_create(volt_ctx, &fullscreen.program, &screen_program_desc);

			ROA_ASSERT(fullscreen.program != VOLT_NULL);
			volt_ctx_execute(volt_ctx); /* because shader text will go out of scope */
		}

		/* triangle */
		{
			float verts[] = {
				/* x y, s t */
				-1.f, +3.f, 0.f, 2.f,
				-1.f, -1.f, 0.f, 0.f,
				+3.f, -1.f, 2.f, 0.f,
			};

			struct volt_vbo_desc vbo_desc;
      ROA_MEM_ZERO(vbo_desc);

			vbo_desc.data  = ROA_ARR_DATA(verts);
			vbo_desc.count = ROA_ARR_COUNT(verts);

			volt_vertex_buffer_create(volt_ctx, &fullscreen.triangle, &vbo_desc);

			ROA_ASSERT(fullscreen.triangle != VOLT_NULL);
			volt_ctx_execute(volt_ctx);
		}

		/* input */
		{
			volt_input_attribute attrs[2];
			attrs[0] = VOLT_INPUT_FLOAT2;
			attrs[1] = VOLT_INPUT_FLOAT2;

			struct volt_input_desc input_desc;
      ROA_MEM_ZERO(input_desc);

			input_desc.attributes = ROA_ARR_DATA(attrs);
			input_desc.count      = ROA_ARR_COUNT(attrs);

			/*volt_input_t input_format;*/
			volt_input_create(volt_ctx, &fullscreen.input, &input_desc);

			ROA_ASSERT(fullscreen.input != VOLT_NULL);
			volt_ctx_execute(volt_ctx); /* attrs will go out of scope */
		}

		/* rasterizer */
		{
			struct volt_rasterizer_desc raster_desc;
      ROA_MEM_ZERO(raster_desc);

			raster_desc.cull_mode 			= VOLT_CULL_FRONT;
			raster_desc.primitive_type 	= VOLT_PRIM_TRIANGLES;
			raster_desc.winding_order 	= VOLT_WIND_CW;

			volt_rasterizer_create(volt_ctx, &fullscreen.rasterizer, &raster_desc);
			volt_ctx_execute(volt_ctx);
		}

		/* pipeline */
		{
			struct volt_pipeline_desc pipeline_desc;
			ROA_MEM_ZERO(pipeline_desc);

			pipeline_desc.viewport 	 = scene.viewport;
			pipeline_desc.input 		 = fullscreen.input;
			pipeline_desc.rasterizer = fullscreen.rasterizer;
			pipeline_desc.program 	 = fullscreen.program;

			fullscreen.pipeline_desc = pipeline_desc;
		}

		/* draw vbo */
		{
			struct volt_draw_desc draw_desc;
			ROA_MEM_ZERO(draw_desc);

			draw_desc.vbo = fullscreen.triangle;

			fullscreen.draw_desc = draw_desc;
		}

    /* sampler */
    {
      struct volt_sampler_desc desc;
      ROA_MEM_ZERO(desc);

      desc.name     = "diffuse";
      desc.sampling = VOLT_SAMPLING_BILINEAR;

      volt_sampler_create(volt_ctx, &fullscreen.u_diffuse, &desc);
    }
	}


  /* ------------------------------------------------------- [ Dir Light ] -- */
  {
    /* program */
    {
      const char vs[] = ""
        "#version 330\n"
        "layout (location = 0) in vec2 Position; \n"
        "layout (location = 1) in vec2 TexC;\n"

        "out vec2 TexCoords;\n"

        "uniform mat4 gWVP;\n"
        "void main()\n"
        "{\n"
        "TexCoords = TexC;\n"
        "gl_Position = vec4(Position, 0.0, 1.0);\n"
        "}\n";

      const char fs[] = ""
        "#version 330\n"

        "struct BaseLight\n"
        "{\n"
        "vec3 Color;\n"
        "float AmbientIntensity;\n"
        "float DiffuseIntensity;\n"
        "};\n"

        "struct DirectionalLight\n"
        "{\n"
        "BaseLight Base;\n"
        "vec3 Direction;\n"
        "};\n"

        "struct Attenuation\n"
        "{\n"
        "float Constant;\n"
        "float Linear;\n"
        "float Exp;\n"
        "};\n"

        "struct PointLight\n"
        "{\n"
        "BaseLight Base;\n"
        "vec3 Position;\n"
        "Attenuation Atten;\n"
        "};\n"

        "struct SpotLight\n"
        "{\n"
        "PointLight Base;\n"
        "vec3 Direction;\n"
        "float Cutoff;\n"
        "};\n"

        "uniform sampler2D gPositionMap;\n"
        "uniform sampler2D gColorMap;\n"
        "uniform sampler2D gNormalMap;\n"

        "uniform DirectionalLight gDirectionalLight;\n"
        "uniform PointLight gPointLight;\n"
        "uniform SpotLight gSpotLight;\n"
        "uniform vec3 gEyeWorldPos;\n"
        "uniform float gMatSpecularIntensity;\n"
        "uniform float gSpecularPower;\n"
        "uniform int gLightType;\n"
        "uniform vec2 gScreenSize;\n"

        "vec4 CalcLightInternal(BaseLight Light,\n"
        "vec3 LightDirection,\n"
        "vec3 WorldPos,\n"
        "vec3 Normal)\n"
        "{\n"
        "vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0);\n"
        "float DiffuseFactor = dot(Normal, -LightDirection);\n"

        "vec4 DiffuseColor  = vec4(0, 0, 0, 0);\n"
        "vec4 SpecularColor = vec4(0, 0, 0, 0);\n"

        "if (DiffuseFactor > 0.0) {\n"
        "DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0);\n"

        "vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos);\n"
        "vec3 LightReflect = normalize(reflect(LightDirection, Normal));\n"
        "float SpecularFactor = dot(VertexToEye, LightReflect); \n"
        "if (SpecularFactor > 0.0) {\n"
        "SpecularFactor = pow(SpecularFactor, gSpecularPower);\n"
        "SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0);\n"
        "}\n"
        "}\n"

        "return (AmbientColor + DiffuseColor + SpecularColor);\n"
        "}\n"

        "vec4 CalcDirectionalLight(vec3 WorldPos, vec3 Normal)\n"
        "{\n"
        "return CalcLightInternal(gDirectionalLight.Base,\n"
        "gDirectionalLight.Direction,\n"
        "WorldPos,\n"
        "Normal);\n"
        "}\n"

        "vec4 CalcPointLight(vec3 WorldPos, vec3 Normal)\n"
        "{\n"
        "vec3 LightDirection = WorldPos - gPointLight.Position;\n"
        "float Distance = length(LightDirection);\n"
        "LightDirection = normalize(LightDirection);\n"

        "vec4 Color = CalcLightInternal(gPointLight.Base, LightDirection, WorldPos, Normal);\n"

        "float Attenuation =  gPointLight.Atten.Constant +\n"
        "gPointLight.Atten.Linear * Distance +\n"
        "gPointLight.Atten.Exp * Distance * Distance;\n"

        "Attenuation = max(1.0, Attenuation);\n"

        "return Color / Attenuation;\n"
        "}\n"

        "vec2 CalcTexCoord()\n"
        "{\n"
        "return gl_FragCoord.xy / gScreenSize;\n"
        "}\n"

        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"

        "void main()\n"
        "{\n"
        "vec2 TexCoord = TexCoords; //CalcTexCoord();\n"
        "vec3 WorldPos = texture(gPositionMap, TexCoord).xyz;\n"
        "vec3 Color = texture(gColorMap, TexCoord).xyz;\n"
        "vec3 Normal = texture(gNormalMap, TexCoord).xyz;\n"
        "Normal = normalize(Normal);\n"
        /*"FragColor = vec4(Color, 1.0) * CalcDirectionalLight(WorldPos, Normal);\n"*/
          "FragColor = vec4(Color, 1.0);\n"
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
      ROA_MEM_ZERO(prog_desc);

      prog_desc.shader_stages_src   = stages_src;
      prog_desc.shader_stages_type  = stages;
      prog_desc.stage_count         = ROA_ARR_COUNT(stages);

      volt_program_create(volt_ctx, &dir_lights.program, &prog_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* triangle */
    {
      float verts[] = {
        /* x y, s t */
        -1.f, +3.f, 0.f, 2.f,
        -1.f, -1.f, 0.f, 0.f,
        +3.f, -1.f, 2.f, 0.f,
      };

      struct volt_vbo_desc vbo_desc;
      ROA_MEM_ZERO(vbo_desc);

      vbo_desc.data   = ROA_ARR_DATA(verts);
      vbo_desc.count  = ROA_ARR_COUNT(verts);

      volt_vertex_buffer_create(volt_ctx, &dir_lights.triangle, &vbo_desc);

      ROA_ASSERT(dir_lights.triangle != VOLT_NULL);
      volt_ctx_execute(volt_ctx);
    }

    /* uniforms */
    {

    }

    /* samplers */
    {
      {
        struct volt_sampler_desc sampler_desc;
        ROA_MEM_ZERO(sampler_desc);

        sampler_desc.name     = "gColorMap";
        sampler_desc.sampling = VOLT_SAMPLING_BILINEAR;

        volt_sampler_create(volt_ctx, &dir_lights.u_color, &sampler_desc);
      }

      {
        struct volt_sampler_desc sampler_desc;
        ROA_MEM_ZERO(sampler_desc);

        sampler_desc.name = "gNormalMap";
        sampler_desc.sampling = VOLT_SAMPLING_BILINEAR;

        volt_sampler_create(volt_ctx, &dir_lights.u_normal, &sampler_desc);
      }

      {
        struct volt_sampler_desc sampler_desc;
        ROA_MEM_ZERO(sampler_desc);

        sampler_desc.name = "gPositionMap";
        sampler_desc.sampling = VOLT_SAMPLING_BILINEAR;

        volt_sampler_create(volt_ctx, &dir_lights.u_position, &sampler_desc);
      }
    }

    /* input format */
    {
      volt_input_attribute attrs[] = {
        VOLT_INPUT_FLOAT2, /* positions */
        VOLT_INPUT_FLOAT2, /* texc */
      };

      struct volt_input_desc input_desc;
      ROA_MEM_ZERO(input_desc);

      input_desc.attributes = ROA_ARR_DATA(attrs);
      input_desc.count      = ROA_ARR_COUNT(attrs);

      /*volt_input_t   input_format;*/
      volt_input_create(volt_ctx, &dir_lights.input, &input_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* rasterizer */
    {
      struct volt_rasterizer_desc raster_desc;
      ROA_MEM_ZERO(raster_desc);

      raster_desc.cull_mode = VOLT_CULL_FRONT;
      raster_desc.primitive_type = VOLT_PRIM_TRIANGLES;
      raster_desc.winding_order = VOLT_WIND_CW;

      volt_rasterizer_create(volt_ctx, &dir_lights.rasterizer, &raster_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* pipeline */
    {
      ROA_MEM_ZERO(dir_lights.pipeline_desc);

      dir_lights.pipeline_desc.program    = dir_lights.program;
      dir_lights.pipeline_desc.input      = dir_lights.input;
      dir_lights.pipeline_desc.rasterizer = dir_lights.rasterizer;
      dir_lights.pipeline_desc.viewport   = scene.viewport;
    }

    /* draw */
    {
      ROA_MEM_ZERO(dir_lights.draw_desc);

      dir_lights.draw_desc.vbo = dir_lights.triangle;
    }
  }

  /* -------------------------------------------------------- [ G-Buffer ] -- */
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

      unsigned i;
      for(i = 0; i < ROA_ARR_COUNT(g_buffer.fbo_color_outputs); ++i)
      {
        struct volt_texture_desc tex_desc;
        ROA_MEM_ZERO(tex_desc);

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
      ROA_MEM_ZERO(tex_desc);

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
      ROA_MEM_ZERO(fbo_desc);

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
      ROA_MEM_ZERO(prog_desc);

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
      ROA_MEM_ZERO(input_desc);

      input_desc.attributes = ROA_ARR_DATA(attrs);
      input_desc.count      = ROA_ARR_COUNT(attrs);

      /*volt_input_t   input_format;*/
      volt_input_create(volt_ctx, &g_buffer.input, &input_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* rasterizer */
    {
      struct volt_rasterizer_desc raster_desc;
      ROA_MEM_ZERO(raster_desc);

      raster_desc.cull_mode       = VOLT_CULL_FRONT;
      raster_desc.primitive_type  = VOLT_PRIM_TRIANGLES;
      raster_desc.winding_order   = VOLT_WIND_CW;

      volt_rasterizer_create(volt_ctx, &g_buffer.rasterizer, &raster_desc);
      volt_ctx_execute(volt_ctx);
    }

		/* pipeline */
		{
			struct volt_pipeline_desc pipe_desc;
			ROA_MEM_ZERO(pipe_desc);

			pipe_desc.program 		= g_buffer.program;
			pipe_desc.rasterizer 	= g_buffer.rasterizer;
			pipe_desc.input 			= g_buffer.input;
			pipe_desc.viewport 		= scene.viewport;

			g_buffer.pipeline_desc = pipe_desc;
		}
	}

  /* ------------------------------------------------------------ [ Loop ] -- */
  while (roa_ctx_new_frame(hw_ctx))
  {
    /* ------------------------------------------- [ Generate Scene Data ] -- */
    {
      /* view mat */
      {
        float radius = 4.f;

        struct roa_ctx_mouse_desc ms_desc;
        ROA_MEM_ZERO(ms_desc);

        roa_ctx_mouse_get_desc(hw_ctx, &ms_desc);

        scene.cam_pitch += ms_desc.y_delta * 0.01f;
        scene.cam_yaw 	+= ms_desc.x_delta * 0.01f;

        float x = roa_float_sin(scene.cam_yaw) * radius;
        float y = 5.5f;
        float z = roa_float_cos(scene.cam_yaw) * radius;

        roa_float3 up  = roa_float3_set_with_values(0, 1, 0);
        roa_float3 pos = roa_float3_set_with_values(x, y, z);
        roa_float3 at  = roa_float3_zero();

        roa_mat4_lookat(&scene.view_mat, pos, at, up);

        scene.cam_position = pos;

        volt_ctx_execute(volt_ctx);
      }

      /* world */
      {
        int count = ROA_ARR_COUNT(scene.box_transform);
        int i;

        static roa_mat4 view_proj;
        roa_mat4_multiply(&view_proj, &scene.view_mat, &scene.proj_mat);

        for (i = 0; i < count; ++i)
        {
          roa_transform_to_mat4(&scene.box_transform[i], &scene.box_world[i]);
          roa_mat4_multiply(&scene.box_wvp[i], &scene.box_world[i], &view_proj);

          volt_ctx_execute(volt_ctx);
        }
      }
    }

    /* ------------------------------------------- [ G Buffer Renderpass ] -- */
    {
      struct volt_renderpass_desc rp_desc;
      ROA_MEM_ZERO(rp_desc);

      rp_desc.fbo  = g_buffer.fbo;
      rp_desc.name = "Fill Gbuffer";

      volt_renderpass_t g_buffer_pass;
      volt_renderpass_create(volt_ctx, &g_buffer_pass, &rp_desc);
			volt_renderpass_set_pipeline(g_buffer_pass, &g_buffer.pipeline_desc);

			unsigned clear_flags = VOLT_CLEAR_COLOR | VOLT_CLEAR_DEPTH;
			volt_renderpass_clear_cmd(g_buffer_pass, clear_flags);

      int i;
      int count = ROA_ARR_COUNT(scene.box_world);

      for (i = 0; i < count; ++i)
      {
				/* Uniform world view projection */
        volt_uniform_t wvp = scene.box_wvp_uni;
				void *wvp_data = (void*)scene.box_wvp[i].data;
				volt_renderpass_bind_uniform_data_cmd(g_buffer_pass, wvp, wvp_data);

				/* Uniform world */
        volt_uniform_t world = scene.box_world_uni;
				void *world_data = (void*)scene.box_world[i].data;
				volt_renderpass_bind_uniform_data_cmd(g_buffer_pass, world, world_data);

				/* Draw */
        volt_renderpass_draw_cmd(g_buffer_pass, &scene.draw_desc[i]);
      }

      volt_renderpass_commit(volt_ctx, &g_buffer_pass);
    }

		/* --------------------------------------- [ Dir Lighting Renderpass ] -- */
		{
      unsigned attachments[] = {
        4,
      };

      struct volt_renderpass_desc rp_desc;
      {
        ROA_MEM_ZERO(rp_desc);

        rp_desc.fbo               = g_buffer.fbo;
        rp_desc.attachments       = ROA_ARR_DATA(attachments);
        rp_desc.attachment_count  = ROA_ARR_COUNT(attachments);
        rp_desc.name              = "Dir Light Pass";
      }

			volt_renderpass_t dir_light_pass;
      volt_renderpass_create(volt_ctx, &dir_light_pass, &rp_desc);
			volt_renderpass_set_pipeline(dir_light_pass, &dir_lights.pipeline_desc);

			unsigned clear_flags = VOLT_CLEAR_COLOR | VOLT_CLEAR_DEPTH;
			volt_renderpass_clear_cmd(dir_light_pass, clear_flags);

      volt_renderpass_bind_texture_buffer_cmd(dir_light_pass, dir_lights.u_position, g_buffer.fbo_color_outputs[0]);
      volt_renderpass_bind_texture_buffer_cmd(dir_light_pass, dir_lights.u_color, g_buffer.fbo_color_outputs[1]);
      volt_renderpass_bind_texture_buffer_cmd(dir_light_pass, dir_lights.u_normal, g_buffer.fbo_color_outputs[2]);

      volt_renderpass_draw_cmd(dir_light_pass, &dir_lights.draw_desc);

			volt_renderpass_commit(volt_ctx, &dir_light_pass);
		}

		/* ---------------------------------------------- [ Final Renderpass ] -- */
		{
			volt_renderpass_t final_pass;
      ROA_MEM_ZERO(final_pass);

			volt_renderpass_create(volt_ctx, &final_pass, 0);
			volt_renderpass_set_pipeline(final_pass, &fullscreen.pipeline_desc);

			int count = ROA_ARR_COUNT(g_buffer.fbo_color_outputs);
			int i;
			int size = width / count;

			/* each color buffer */
			for(i = 0; i < count; ++i)
			{
				unsigned clear_flags = VOLT_CLEAR_COLOR | VOLT_CLEAR_DEPTH;
        volt_renderpass_clear_cmd(final_pass, clear_flags);

        struct volt_rect2d scissor = { size * i, 0, size, height};
				volt_renderpass_set_scissor_cmd(final_pass, scissor);
				volt_renderpass_bind_texture_buffer_cmd(final_pass, fullscreen.u_diffuse, g_buffer.fbo_color_outputs[i]);

				volt_renderpass_draw_cmd(final_pass, &fullscreen.draw_desc);
			}

			volt_renderpass_commit(volt_ctx, &final_pass);
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
