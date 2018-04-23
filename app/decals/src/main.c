#include <roa_ctx/roa_ctx.h>
#include <roa_lib/assert.h>
#include <roa_lib/alloc.h>
#include <roa_lib/fundamental.h>
#include <roa_math/math.h>
#include <volt/volt.h>
#include <roa_lib/time.h>
#include <scratch/geometry.h>
#include <scratch/glsl.h>
#include <stdlib.h>
#include <stdio.h>


/* ------------------------------------------------------------ [ Config ] -- */


#define DECAL_TEST_SHOW_G_BUFFER 0


/* ----------------------------------------------------------- [ Systems ] -- */


roa_ctx_t  hw_ctx;     /* windowing / input */
volt_ctx_t volt_ctx;   /* graphics api */


/* ------------------------------------------------------------- [ Scene ] -- */


int width  = 1200;
int height = 720;


struct g_buffer_data
{
  volt_framebuffer_t  fbo;
  volt_texture_t      fbo_color_outputs[5];
  volt_texture_t      fbo_depth;

  /* object render */
	volt_program_t      program;
	volt_input_t        input;
	volt_rasterizer_t   rasterizer;

	struct volt_pipeline_desc pipeline_desc;

  /* decal render */
  volt_program_t      decal_program;
	volt_input_t        decal_input;

	volt_uniform_t      u_inv_proj_view;
	volt_uniform_t      u_inv_model;
	volt_uniform_t      u_view;
	volt_uniform_t      u_proj;
	volt_uniform_t      u_model;
	volt_uniform_t      u_decal_size;
	volt_uniform_t      u_far_clip;

	volt_sampler_t      s_normal;
	volt_sampler_t      s_depth;
	volt_sampler_t      s_diffuse;

  struct volt_pipeline_desc decal_pipeline_desc;

} g_buffer;


struct point_light_data
{
  volt_program_t      program;
  volt_rasterizer_t   rasterizer;
  volt_input_t        input;
} point_lights;


struct direction_light_data
{
  volt_program_t    program;
  volt_rasterizer_t rasterizer;
  volt_vbo_t        triangle;
  volt_input_t      input;

  volt_uniform_t    u_eye_pos;
  volt_uniform_t    u_dir_light_color;
  volt_uniform_t    u_dir_light_amb_int;
  volt_uniform_t    u_dir_light_diff_int;
  volt_uniform_t    u_dir_light_dir;
  volt_uniform_t    u_spec_power;
  volt_uniform_t    u_mat_spec_intensity;
  volt_uniform_t    u_light_type;
  volt_uniform_t    u_screen_size;

  volt_sampler_t    u_color;
  volt_sampler_t    u_position;
  volt_sampler_t    u_normal;

  struct volt_pipeline_desc pipeline_desc;
  struct volt_draw_desc     draw_desc;

} dir_lights;


struct scene_data
{
  /* objects */

  volt_vbo_t            object_vbo;
  roa_transform         box_transform[1];
  roa_mat4              box_world[1];
  roa_mat4              box_wvp[1];
  struct volt_draw_desc box_draw_desc[1];

  /* decals */

  volt_vbo_t            decal_vbo;
  roa_transform         decal_transform[1];
  roa_mat4              decal_world[1];
  roa_mat4              decal_wvp[1];
  roa_mat4              decal_inv_world[1];
	roa_mat4 							decal_inv_proj_view[1];
  roa_mat4              decal_inv_view[1];
  roa_mat4              decal_world_view[1];
	roa_float3            decal_size[1];
	float                 far_clip[1];
  struct volt_draw_desc decal_draw_desc[1];

  /* camera */

  struct volt_rect2d    viewport;
  volt_uniform_t        box_world_uni;
  volt_uniform_t        box_wvp_uni;

  roa_mat4              view_mat;
  roa_mat4              proj_mat;
  roa_mat4              inv_view_mat;

  roa_float3            cam_position;
  float                 cam_pitch;
  float                 cam_yaw;
  float                 cam_radius;
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
    scene.viewport.x      = 0;
    scene.viewport.y      = 0;
    scene.viewport.width  = width;
    scene.viewport.height = height;

    /* load object vbo */
    {
      geom_vert_desc desc[] = {
        GEOM_VERT_POSITION3,
        GEOM_UV,
        GEOM_NORMAL,
      };

      unsigned vert_count;

      geometry_generate_cube(desc, 3, 1, 1, 1, ROA_NULL, &vert_count);

      float *data = malloc(sizeof(float) * vert_count);

      geometry_generate_cube(desc, 3, 1, 1, 1, data, &vert_count);

      struct volt_vbo_desc vbo_desc;
      ROA_MEM_ZERO(vbo_desc);
			vbo_desc.name  = "Cube";
      vbo_desc.data  = data;
      vbo_desc.count = vert_count;

      volt_vertex_buffer_create(volt_ctx, &scene.object_vbo, &vbo_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* draw obj desc */
    {
      int count = ROA_ARR_COUNT(scene.box_draw_desc);
      int i;

      for(i = 0; i < count; ++i)
      {
        ROA_MEM_ZERO(scene.box_draw_desc[i]);
        scene.box_draw_desc[i].vbo = scene.object_vbo;
      }
    }

    /* load decal vbo */
    {
      geom_vert_desc desc[] = {
        GEOM_VERT_POSITION4,
        GEOM_UV,
        GEOM_NORMAL,
      };

      unsigned vert_count;

      geometry_generate_cube(desc, 3, 1, 1, 1, ROA_NULL, &vert_count);

      float *data = malloc(sizeof(float) * vert_count);

      geometry_generate_cube(desc, 3, 1, 1, 1, data, &vert_count);

      struct volt_vbo_desc vbo_desc;
      ROA_MEM_ZERO(vbo_desc);
			vbo_desc.name  = "DecalBox";
      vbo_desc.data  = data;
      vbo_desc.count = vert_count;

      volt_vertex_buffer_create(volt_ctx, &scene.decal_vbo, &vbo_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* draw decal desc */
    {
      int count = ROA_ARR_COUNT(scene.decal_draw_desc);
      int i;

      for (i = 0; i < count; ++i)
      {
        ROA_MEM_ZERO(scene.decal_draw_desc[i]);
        scene.decal_draw_desc[i].vbo = scene.decal_vbo;
      }
    }

    /* box positions */
    {
      roa_transform_init(&scene.box_transform[0]);
      scene.box_transform[0].position = roa_float3_set_with_values(0.f, ROA_G_RATIO / 2, 0.f);
      scene.box_transform[0].scale    = roa_float3_set_with_values(ROA_G_RATIO / 4, ROA_G_RATIO, 1.f);
    }

    /* decals */
    {
      roa_transform_init(&scene.decal_transform[0]);
      scene.decal_transform[0].position = roa_float3_set_with_values(0,0,0);
      scene.decal_transform[0].scale    = roa_float3_set_with_values(2, 2, 2);
    }

    /* uniforms */
    {
      struct volt_uniform_desc wvp_uni_desc;
      {
        ROA_MEM_ZERO(wvp_uni_desc);

        wvp_uni_desc.name       = "gWVP";
        wvp_uni_desc.data_type  = VOLT_DATA_MAT4;
        wvp_uni_desc.count      = 1;
        wvp_uni_desc.copy_data  = VOLT_FALSE;

        volt_uniform_create(volt_ctx, &scene.box_wvp_uni, &wvp_uni_desc);
      }

      struct volt_uniform_desc world_uni_desc;
      {
        ROA_MEM_ZERO(world_uni_desc);
        world_uni_desc.name       = "gWorld";
        world_uni_desc.data_type  = VOLT_DATA_MAT4;
        world_uni_desc.count      = 1;
        world_uni_desc.copy_data  = VOLT_FALSE;

        volt_uniform_create(volt_ctx, &scene.box_world_uni, &world_uni_desc);
      }

      volt_ctx_execute(volt_ctx);
    }

    /* projection camera */
    {
      float aspect  = (float)width / (float)height;
      float fov     = ROA_TAU * 0.125f;

      roa_mat4_projection(&scene.proj_mat, fov, 0.1, 100, aspect);

      scene.cam_pitch    = 0.f;
      scene.cam_yaw      = 0.f;
      scene.cam_radius   = 4.f;
      scene.cam_position = roa_float3_one();
    }
  }

  /* ------------------------------------------------------ [ Fullscreen ] -- */
  {
    /* program */
    {
      const char *shaders[2];
      shaders[0] = glsl_blit_vs();
      shaders[1] = glsl_blit_fs();

      volt_shader_stage stages[] = {
        VOLT_SHD_VERTEX,
        VOLT_SHD_FRAGMENT,
      };

      struct volt_program_desc screen_program_desc;
      ROA_MEM_ZERO(screen_program_desc);
			screen_program_desc.name                = "Scratch:Blit";
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
        /* x y, z, s t */
        -1.f, +3.f, 0.f, 0.f, 2.f,
        -1.f, -1.f, 0.f, 0.f, 0.f,
        +3.f, -1.f, 0.f, 2.f, 0.f,
      };

      struct volt_vbo_desc vbo_desc;
      ROA_MEM_ZERO(vbo_desc);
			vbo_desc.name  = "FullscreenTriangle:XYZST";
      vbo_desc.data  = ROA_ARR_DATA(verts);
      vbo_desc.count = ROA_ARR_COUNT(verts);

      volt_vertex_buffer_create(volt_ctx, &fullscreen.triangle, &vbo_desc);

      ROA_ASSERT(fullscreen.triangle != VOLT_NULL);
      volt_ctx_execute(volt_ctx);
    }

    /* input */
    {
      volt_input_attribute attrs[2];
      attrs[0] = VOLT_INPUT_FLOAT3;
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
      raster_desc.cull_mode       = VOLT_CULL_BACK;
      raster_desc.primitive  			= VOLT_PRIM_TRIANGLES;
      raster_desc.winding_order   = VOLT_WIND_CCW;
			raster_desc.depth_test      = VOLT_TRUE;

      volt_rasterizer_create(volt_ctx, &fullscreen.rasterizer, &raster_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* pipeline */
    {
      struct volt_pipeline_desc pipeline_desc;
      ROA_MEM_ZERO(pipeline_desc);
      pipeline_desc.viewport   = scene.viewport;
      pipeline_desc.input      = fullscreen.input;
      pipeline_desc.rasterizer = fullscreen.rasterizer;
      pipeline_desc.program    = fullscreen.program;

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
      desc.name     = "samp_diffuse_01";
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
        "FragColor = (vec4(Color, 1.0) * 0.1) + vec4(Color, 1.0) * CalcDirectionalLight(WorldPos, Normal);\n"
         /* "FragColor = vec4(Color, 1.0);\n"*/
        "}\n";


      volt_shader_stage stages[2] = {
        VOLT_SHD_VERTEX,
        VOLT_SHD_FRAGMENT,
      };

      const char *stages_src[2];
      stages_src[0] = vs;
      stages_src[1] = fs;

      struct volt_program_desc prog_desc;
      ROA_MEM_ZERO(prog_desc);
			prog_desc.name                = "Lighting:Directional";
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
			vbo_desc.name   = "FullscreenTriangle:XYST";
      vbo_desc.data   = ROA_ARR_DATA(verts);
      vbo_desc.count  = ROA_ARR_COUNT(verts);

      volt_vertex_buffer_create(volt_ctx, &dir_lights.triangle, &vbo_desc);

      ROA_ASSERT(dir_lights.triangle != VOLT_NULL);
      volt_ctx_execute(volt_ctx);
    }

    /* uniforms */
    {
      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);

        uni_desc.name       = "gEyeWorldPos";
        uni_desc.count      = 1;
        uni_desc.copy_data  = VOLT_FALSE;
        uni_desc.data_type  = VOLT_DATA_FLOAT3;

        volt_uniform_create(volt_ctx, &dir_lights.u_eye_pos, &uni_desc);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);

        uni_desc.name       = "gMatSpecularIntensity";
        uni_desc.count      = 1;
        uni_desc.copy_data  = VOLT_FALSE;
        uni_desc.data_type  = VOLT_DATA_FLOAT;

        volt_uniform_create(volt_ctx, &dir_lights.u_mat_spec_intensity, &uni_desc);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);

        uni_desc.name       = "gSpecularPower";
        uni_desc.count      = 1;
        uni_desc.copy_data  = VOLT_FALSE;
        uni_desc.data_type  = VOLT_DATA_FLOAT;

        volt_uniform_create(volt_ctx, &dir_lights.u_spec_power, &uni_desc);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);

        uni_desc.name       = "gLightType";
        uni_desc.count      = 1;
        uni_desc.copy_data  = VOLT_FALSE;
        uni_desc.data_type  = VOLT_DATA_INT;

        volt_uniform_create(volt_ctx, &dir_lights.u_light_type, &uni_desc);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);

        uni_desc.name       = "gScreenSize";
        uni_desc.count      = 1;
        uni_desc.copy_data  = VOLT_FALSE;
        uni_desc.data_type  = VOLT_DATA_FLOAT2;

        volt_uniform_create(volt_ctx, &dir_lights.u_screen_size, &uni_desc);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);

        uni_desc.name = "gDirectionalLight.Base.Color";
        uni_desc.count = 1;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.data_type = VOLT_DATA_FLOAT3;

        volt_uniform_create(volt_ctx, &dir_lights.u_dir_light_color, &uni_desc);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);

        uni_desc.name = "gDirectionalLight.Base.AmbientIntensity";
        uni_desc.count = 1;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.data_type = VOLT_DATA_FLOAT;

        volt_uniform_create(volt_ctx, &dir_lights.u_dir_light_amb_int, &uni_desc);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);

        uni_desc.name = "gDirectionalLight.Base.DiffuseIntensity";
        uni_desc.count = 1;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.data_type = VOLT_DATA_FLOAT;

        volt_uniform_create(volt_ctx, &dir_lights.u_dir_light_diff_int, &uni_desc);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);
        uni_desc.name = "gDirectionalLight.Direction";
        uni_desc.count = 1;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.data_type = VOLT_DATA_FLOAT3;

        volt_uniform_create(volt_ctx, &dir_lights.u_dir_light_dir, &uni_desc);
      }
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
      raster_desc.cull_mode 		 = VOLT_CULL_BACK;
      raster_desc.primitive	 		 = VOLT_PRIM_TRIANGLES;
      raster_desc.winding_order  = VOLT_WIND_CCW;
			raster_desc.blend_equation = VOLT_BLEND_EQ_ADD;
			raster_desc.blend_src      = VOLT_BLEND_FLAG_ONE;
			raster_desc.blend_dst      = VOLT_BLEND_FLAG_ONE;
			raster_desc.depth_test 		 = VOLT_TRUE;

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
			fbo_desc.name             = "FBO:GBuffer";
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
        "layout(location = 1) in vec2 TexCoord; \n"
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

        "uniform sampler2D gColorMap;\n"

        "layout (location = 0) out vec3 WorldPosOut;\n"
        "layout (location = 1) out vec3 DiffuseOut;\n"
        "layout (location = 2) out vec3 NormalOut;\n"
        "layout (location = 3) out vec3 TexCoordOut;\n"

        "void main()  \n"
        "{\n"
        " WorldPosOut     = WorldPos0;\n"
        " DiffuseOut      = vec3(1, 0.3, 0); /*texture(gColorMap, TexCoord0).xyz;*/\n"
        " NormalOut       = normalize(Normal0);\n"
        " TexCoordOut     = vec3(TexCoord0, 0.0);\n"
        "}\n";

      volt_shader_stage stages[2] = {
        VOLT_SHD_VERTEX,
        VOLT_SHD_FRAGMENT,
      };

      const char *stages_src[2];
      stages_src[0] = vs;
      stages_src[1] = fs;

      struct volt_program_desc prog_desc;
      ROA_MEM_ZERO(prog_desc);
			prog_desc.name                = "GBuffer:Fill";
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

    /* decal_input input format */
    {
      volt_input_attribute attrs[3] = {
        VOLT_INPUT_FLOAT4, /* positions */
        VOLT_INPUT_FLOAT2, /* tex coords  */
        VOLT_INPUT_FLOAT3, /* normals */
      };

      struct volt_input_desc input_desc;
      ROA_MEM_ZERO(input_desc);

      input_desc.attributes = ROA_ARR_DATA(attrs);
      input_desc.count      = ROA_ARR_COUNT(attrs);

      /*volt_input_t   input_format;*/
      volt_input_create(volt_ctx, &g_buffer.decal_input, &input_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* rasterizer */
    {
      struct volt_rasterizer_desc raster_desc;
      ROA_MEM_ZERO(raster_desc);

      raster_desc.cull_mode     = VOLT_CULL_BACK;
      raster_desc.primitive  		= VOLT_PRIM_TRIANGLES;
      raster_desc.winding_order = VOLT_WIND_CCW;
			raster_desc.depth_test    = VOLT_TRUE;

      volt_rasterizer_create(volt_ctx, &g_buffer.rasterizer, &raster_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* pipeline */
    {
      struct volt_pipeline_desc pipe_desc;
      ROA_MEM_ZERO(pipe_desc);

      pipe_desc.program 	 = g_buffer.program;
      pipe_desc.rasterizer = g_buffer.rasterizer;
      pipe_desc.input 		 = g_buffer.input;
      pipe_desc.viewport 	 = scene.viewport;

      g_buffer.pipeline_desc = pipe_desc;
    }

    /* decal program */
    {
      const char vs[] = ""
        "#version 430\n"

        "layout(location=0) in vec4 vs_in_position;\n"
        "layout(location=1) in vec3 vs_in_normal;\n"
        "layout(location=2) in vec2 vs_in_texcoord;\n"

        "uniform mat4 u_view;\n"
        "uniform mat4 u_proj;\n"
        "uniform mat4 u_model;\n"
        "uniform vec3 u_decal_size;\n"
        "uniform float u_far_clip;\n"

        "out vec4 pos_fs;\n"
        "out vec4 pos_w;\n"
        "out vec2 uv_fs;\n"

        "void main()\n"
        "{\n"
        " pos_w       = u_model * vec4(vs_in_position.xyz * 1, vs_in_position.w);\n"
        " pos_fs      = u_proj * u_view * pos_w;\n"
        " uv_fs       = vs_in_texcoord;\n"
        " gl_Position = pos_fs;\n"
        "}\n";

      const char fs[] = ""
        "#version 430\n"
        "#extension GL_ARB_texture_rectangle : enable\n"

        "in vec4 pos_fs;\n"
        "in vec4 pos_w;\n"
        "in vec2 uv_fs;\n"

        "uniform sampler2D samp_normal;\n"
        "uniform sampler2D samp_depth;\n"
        "uniform sampler2D samp_diffuse;\n"

        "uniform mat4 u_inv_proj_view;\n"
        "uniform mat4 u_inv_model;\n"

        "vec4 reconstruct_pos(float z, vec2 uv_f)\n"
        "{\n"
        "  vec4 sPos = vec4(uv_f * 2.0 - 1.0, z, 1.0);\n"
          "sPos = u_inv_proj_view * sPos;\n"
          "return vec4((sPos.xyz / sPos.w), sPos.w);\n"
        "}\n"

        "layout (location = 0) out vec3 fs_out_diffuse;\n"

        "void main()  \n"
        "{\n"
        "vec2 screenPosition = pos_fs.xy / pos_fs.w;\n"

        "vec2 depthUV = screenPosition * 0.5 + 0.5;\n"
        "depthUV += vec2(0.5 / 1200.0, 0.5 / 720.0);\n"
        "float depth = texture2D(samp_depth, depthUV).w;\n"

        "vec4 worldPos = reconstruct_pos(depth, depthUV);\n"
        "vec4 localPos = u_inv_model * worldPos;\n"

        "float dist = 0.5 - abs(localPos.y);\n"
        "float dist2 = 0.5 - abs(localPos.x);\n"

        "if (dist > 0.0f && dist2 > 0)\n"
        "{\n"
          "vec2 uv = vec2(localPos.x, localPos.y) + 0.5;\n"
          "vec4 diffuseColor = texture2D(samp_diffuse, uv);\n"
          "fs_out_diffuse = diffuseColor.xyz;\n"
        "}\n"
        "else {\n"
          "fs_out_diffuse = vec4(1.0, 0, 0, 1).xyz;\n"
          "}\n"
        "}\n";

      volt_shader_stage stages[2] = {
        VOLT_SHD_VERTEX,
        VOLT_SHD_FRAGMENT,
      };

      const char *stages_src[2];
      stages_src[0] = vs;
      stages_src[1] = fs;

      struct volt_program_desc prog_desc;
      ROA_MEM_ZERO(prog_desc);
			prog_desc.name                = "GBuffer:Decal";
      prog_desc.shader_stages_src   = stages_src;
      prog_desc.shader_stages_type  = stages;
      prog_desc.stage_count 				= ROA_ARR_COUNT(stages);

      volt_program_create(volt_ctx, &g_buffer.decal_program, &prog_desc);
      volt_ctx_execute(volt_ctx);
    }

    /* uniforms and samplers */
    {
      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);
        uni_desc.name = "u_inv_proj_view";
        uni_desc.data_type = VOLT_DATA_MAT4;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &g_buffer.u_inv_proj_view, &uni_desc);
        volt_ctx_execute(volt_ctx);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);
        uni_desc.name = "u_inv_model";
        uni_desc.data_type = VOLT_DATA_MAT4;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &g_buffer.u_inv_model, &uni_desc);
        volt_ctx_execute(volt_ctx);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);
        uni_desc.name = "u_view";
        uni_desc.data_type = VOLT_DATA_MAT4;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &g_buffer.u_view, &uni_desc);
        volt_ctx_execute(volt_ctx);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);
        uni_desc.name = "u_proj";
        uni_desc.data_type = VOLT_DATA_MAT4;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &g_buffer.u_proj, &uni_desc);
        volt_ctx_execute(volt_ctx);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);
        uni_desc.name = "u_model";
        uni_desc.data_type = VOLT_DATA_MAT4;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &g_buffer.u_model, &uni_desc);
        volt_ctx_execute(volt_ctx);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);
        uni_desc.name = "u_decal_size";
        uni_desc.data_type = VOLT_DATA_FLOAT3;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &g_buffer.u_decal_size, &uni_desc);
        volt_ctx_execute(volt_ctx);
      }

      {
        struct volt_uniform_desc uni_desc;
        ROA_MEM_ZERO(uni_desc);
        uni_desc.name = "u_far_clip";
        uni_desc.data_type = VOLT_DATA_FLOAT;
        uni_desc.copy_data = VOLT_FALSE;
        uni_desc.count = 1;

        volt_uniform_create(volt_ctx, &g_buffer.u_far_clip, &uni_desc);
        volt_ctx_execute(volt_ctx);
      }

      {
        struct volt_sampler_desc samp_desc;
        ROA_MEM_ZERO(samp_desc);
        samp_desc.name = "samp_normal";
        samp_desc.sampling = VOLT_SAMPLING_BILINEAR;

        volt_sampler_create(volt_ctx, &g_buffer.s_normal, &samp_desc);
      }

      {
        struct volt_sampler_desc samp_desc;
        ROA_MEM_ZERO(samp_desc);
        samp_desc.name = "samp_depth";
        samp_desc.sampling = VOLT_SAMPLING_BILINEAR;

        volt_sampler_create(volt_ctx, &g_buffer.s_depth, &samp_desc);
      }

      {
        struct volt_sampler_desc samp_desc;
        ROA_MEM_ZERO(samp_desc);
        samp_desc.name = "samp_diffuse";
        samp_desc.sampling = VOLT_SAMPLING_BILINEAR;

        volt_sampler_create(volt_ctx, &g_buffer.s_diffuse, &samp_desc);
      }
    }

    /* decal pipeline */
    {
      struct volt_pipeline_desc decal_pipeline_desc;
      ROA_MEM_ZERO(decal_pipeline_desc);

      decal_pipeline_desc.program     = g_buffer.decal_program;
      decal_pipeline_desc.rasterizer  = g_buffer.rasterizer;
      decal_pipeline_desc.input       = g_buffer.decal_input;
      decal_pipeline_desc.viewport    = scene.viewport;

      g_buffer.decal_pipeline_desc = decal_pipeline_desc;
    }
  }

  /* --------------------------------------------------------- [ App Loop ] -- */
  while (roa_ctx_new_frame(hw_ctx))
  {
    /* ------------------------------------------- [ Generate Scene Data ] -- */
    {
      /* view mat */
      {
				/* camera movement */
				{
	        struct roa_ctx_mouse_desc ms_desc;
	        ROA_MEM_ZERO(ms_desc);

	        roa_ctx_mouse_get_desc(hw_ctx, &ms_desc);

	        scene.cam_pitch  += ms_desc.y_delta * 0.02f;
	        scene.cam_yaw 	 += ms_desc.x_delta * 0.02f;
					scene.cam_radius += ms_desc.y_scroll * 0.4f;
				}

				/* calculate camera lookat */
				{
					roa_float3 w_up	 	= roa_transform_world_up();
					roa_float3 w_left = roa_transform_world_left();
					roa_float3 w_fwd 	= roa_transform_world_fwd();

					roa_quaternion yaw 					= roa_quaternion_from_axis_angle(w_up, scene.cam_yaw);
					roa_quaternion pitch 				= roa_quaternion_from_axis_angle(w_left, scene.cam_pitch);
					roa_quaternion rot 					= roa_quaternion_multiply(yaw, pitch);

					roa_float3 rotated_position = roa_quaternion_rotate_vector(rot, w_fwd);
					roa_float3 scaled_position 	= roa_float3_scale(rotated_position, scene.cam_radius);
					roa_float3 local_up 				= roa_quaternion_rotate_vector(rot, w_up);

	        roa_mat4_lookat(&scene.view_mat, scaled_position, roa_float3_zero(), local_up);

					/* update eye position */
	        scene.cam_position = scaled_position;
				}
      }

      /* cam data */
      static roa_mat4 view_proj;
      roa_mat4_multiply(&view_proj, &scene.view_mat, &scene.proj_mat);

      /* object data */
      {
        int count = ROA_ARR_COUNT(scene.box_transform);
        int i;

        for (i = 0; i < count; ++i)
        {
          roa_transform_to_mat4(&scene.box_transform[i], &scene.box_world[i]);
          roa_mat4_multiply(&scene.box_wvp[i], &scene.box_world[i], &view_proj);

          volt_ctx_execute(volt_ctx);
        }
      }

      /* decal data */
      {
        int count = ROA_ARR_COUNT(scene.decal_transform);
        int i;

        for (i = 0; i < count; ++i)
        {
          roa_transform_to_mat4(&scene.decal_transform[i], &scene.decal_world[i]);
          roa_mat4_multiply(&scene.decal_wvp[i], &scene.decal_world[i], &view_proj);

          roa_mat4_multiply(&scene.decal_world_view[i], &scene.decal_world[i], &scene.view_mat);

          roa_mat4_inverse(&scene.decal_inv_world[i], &scene.decal_world[i]);

          roa_mat4 proj_view;
          roa_mat4_multiply(&proj_view, &scene.proj_mat, &scene.view_mat);

          roa_mat4_inverse(&scene.decal_inv_proj_view[i], &proj_view);

          roa_mat4_inverse(&scene.decal_inv_view[i], &scene.view_mat);

          scene.decal_size[i] = roa_float3_set_with_values(1,1,1);
          scene.far_clip[i] = 10.f;

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
        volt_renderpass_draw_cmd(g_buffer_pass, &scene.box_draw_desc[i]);
      }

      volt_renderpass_commit(volt_ctx, &g_buffer_pass);
    }

    /* --------------------------------------------- [ Decals Renderpass ] -- */
    {
      unsigned attachments[] = {
        1,
      };

      struct volt_renderpass_desc decal_pass_desc;
      ROA_MEM_ZERO(decal_pass_desc);
      decal_pass_desc.fbo = g_buffer.fbo;
      decal_pass_desc.name = "Decal Gbuffer";
      decal_pass_desc.attachments = ROA_ARR_DATA(attachments);
      decal_pass_desc.attachment_count = ROA_ARR_COUNT(attachments);

      volt_renderpass_t decal_pass;
      volt_renderpass_create(volt_ctx, &decal_pass, &decal_pass_desc);
      volt_renderpass_set_pipeline(decal_pass, &g_buffer.decal_pipeline_desc);

      int i;
      int count = ROA_ARR_COUNT(scene.decal_world);

      for (i = 0; i < count; ++i)
      {
        volt_uniform_t u_view = g_buffer.u_view;
        void *u_view_data = (void*)scene.view_mat.data;
        volt_renderpass_bind_uniform_data_cmd(decal_pass, u_view, u_view_data);

        volt_uniform_t u_proj = g_buffer.u_proj;
        void *u_proj_data = (void*)scene.proj_mat.data;
        volt_renderpass_bind_uniform_data_cmd(decal_pass, u_proj, u_proj_data);

        volt_uniform_t u_model = g_buffer.u_model;
        void *u_model_data = (void*)scene.decal_world[i].data;
        volt_renderpass_bind_uniform_data_cmd(decal_pass, u_model, u_model_data);

        volt_uniform_t u_inv_proj_view = g_buffer.u_inv_proj_view;
        void *u_inv_proj_view_data = (void*)scene.decal_inv_proj_view;
        volt_renderpass_bind_uniform_data_cmd(decal_pass, u_inv_proj_view, u_inv_proj_view_data);

        volt_uniform_t u_inv_model = g_buffer.u_inv_model;
        void *u_inv_model_data = (void*)scene.decal_inv_world;
        volt_renderpass_bind_uniform_data_cmd(decal_pass, u_inv_model, u_inv_model_data);

        volt_uniform_t u_decal_size = g_buffer.u_decal_size;
        void *u_decal_size_data = (void*)scene.decal_size;
        volt_renderpass_bind_uniform_data_cmd(decal_pass, u_decal_size, u_decal_size_data);

        volt_uniform_t u_far_clip = g_buffer.u_far_clip;
        void *u_far_clip_data = (void*)scene.far_clip;
        volt_renderpass_bind_uniform_data_cmd(decal_pass, u_far_clip, u_far_clip_data);

        volt_sampler_t samp_normal = g_buffer.s_normal;
        volt_renderpass_bind_texture_buffer_cmd(decal_pass, samp_normal, g_buffer.fbo_color_outputs[2]);

        volt_sampler_t samp_depth = g_buffer.s_depth;
        volt_renderpass_bind_texture_buffer_cmd(decal_pass, samp_depth, g_buffer.fbo_depth);

        volt_sampler_t samp_diffuse = g_buffer.s_diffuse;
        volt_renderpass_bind_texture_buffer_cmd(decal_pass, samp_diffuse, g_buffer.fbo_color_outputs[1]);

        /* Draw */
        volt_renderpass_draw_cmd(decal_pass, &scene.decal_draw_desc[i]);
      }

      volt_renderpass_commit(volt_ctx, &decal_pass);
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

      static float amb_intensity  = 0.f;
      static float diff_intensity = 0.5f;
      static float color[3]       = {1,1,0};
      static float direction[3]   = {0.4558f, -0.6837f, 0.5698f};

      volt_renderpass_t dir_light_pass;
      volt_renderpass_create(volt_ctx, &dir_light_pass, &rp_desc);
      volt_renderpass_set_pipeline(dir_light_pass, &dir_lights.pipeline_desc);

      unsigned clear_flags = VOLT_CLEAR_COLOR | VOLT_CLEAR_DEPTH;
      volt_renderpass_clear_cmd(dir_light_pass, clear_flags);

      volt_renderpass_bind_uniform_data_cmd(dir_light_pass, dir_lights.u_dir_light_color, &color);
      volt_renderpass_bind_uniform_data_cmd(dir_light_pass, dir_lights.u_dir_light_amb_int, &amb_intensity);
      volt_renderpass_bind_uniform_data_cmd(dir_light_pass, dir_lights.u_dir_light_diff_int, &diff_intensity);
      volt_renderpass_bind_uniform_data_cmd(dir_light_pass, dir_lights.u_dir_light_dir, &direction);

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

      unsigned clear_flags = VOLT_CLEAR_COLOR | VOLT_CLEAR_DEPTH;
      volt_renderpass_clear_cmd(final_pass, clear_flags);

      if(ROA_IS_ENABLED(DECAL_TEST_SHOW_G_BUFFER))
      {
        int count = ROA_ARR_COUNT(g_buffer.fbo_color_outputs);
        int i;
        int size = width / count;

        /* each color buffer */
        for(i = 0; i < count; ++i)
        {
          struct volt_rect2d scissor;
          scissor.x       = size * i;
          scissor.y       = 0;
          scissor.width   = width;
          scissor.height  = height;

          volt_renderpass_set_scissor_cmd(final_pass, scissor);
          volt_renderpass_bind_texture_buffer_cmd(final_pass, fullscreen.u_diffuse, g_buffer.fbo_color_outputs[i]);

          volt_renderpass_draw_cmd(final_pass, &fullscreen.draw_desc);
        }
      }
      else
      {
        volt_renderpass_bind_texture_buffer_cmd(final_pass, fullscreen.u_diffuse, g_buffer.fbo_color_outputs[4]);

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
