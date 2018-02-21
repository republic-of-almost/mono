
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <volt/volt.h>
#include <roa_lib/fundamental.h>
#include <roa_lib/dir.h>
#include <roa_lib/assert.h>
#include <roa_math/math.h>
#include <string.h>


volt_ctx_t ctx               = VOLT_NULL;
volt_vbo_t vbo               = VOLT_NULL;
volt_ibo_t ibo               = VOLT_NULL;
volt_texture_t texture_1     = VOLT_NULL;
volt_texture_t texture_2     = VOLT_NULL;
volt_program_t program       = VOLT_NULL;
volt_uniform_t world_data    = VOLT_NULL;
volt_uniform_t proj_data     = VOLT_NULL;
volt_uniform_t view_data     = VOLT_NULL;
volt_input_t vert_input      = VOLT_NULL;
volt_rasterizer_t rasterizer = VOLT_NULL;


void
test_cube_create()
{
  volt_ctx_create(&ctx);

  roa_mat4 world;
  roa_mat4 view;
  roa_mat4 proj;

  /* create uniforms */
  {
    struct volt_uniform_desc uni_desc_1;
    uni_desc_1.data_type = VOLT_DATA_MAT4;
    uni_desc_1.count = 1;

    volt_uniform_create(ctx, &proj_data, &uni_desc_1);
    ROA_ASSERT(proj_data);

    struct volt_uniform_desc uni_desc_2;
    uni_desc_2.data_type = VOLT_DATA_MAT4;
    uni_desc_2.count = 1;
    
    volt_uniform_create(ctx, &view_data, &uni_desc_2);
    ROA_ASSERT(view_data);

    struct volt_uniform_desc uni_desc_3;
    uni_desc_3.data_type = VOLT_DATA_MAT4;
    uni_desc_3.count = 1;
    
    volt_uniform_create(ctx, &world_data, &uni_desc_2);
    ROA_ASSERT(world_data);
  }

  /* create some assets */
  {
    int x,y,n;

    /* textures */
    struct volt_texture_desc tex_desc_1;
    tex_desc_1.dimentions = VOLT_TEXTURE_2D;
    tex_desc_1.mip_maps = VOLT_FALSE;
    tex_desc_1.sampling = VOLT_SAMPLING_BILINEAR;

    char file_path_01[2048];
    memset(file_path_01, 0, sizeof(file_path_01));
    strcat(file_path_01, roa_exe_path());
    strcat(file_path_01, "assets/volt_func/dev_tex_01.png");

    tex_desc_1.data = (void*)stbi_load(
      file_path_01,
      &x,
      &y,
      &n,
      0
    );

    tex_desc_1.width = x;
    tex_desc_1.height = y;
    tex_desc_1.format = n > 3 ? VOLT_COLOR_RGBA : VOLT_COLOR_RGB;

    struct volt_texture_desc tex_desc_2;
    tex_desc_2.dimentions = VOLT_TEXTURE_2D;
    tex_desc_2.mip_maps = VOLT_FALSE;
    tex_desc_2.sampling = VOLT_SAMPLING_BILINEAR;

    char file_path_02[2048];
    memset(file_path_02, 0, sizeof(file_path_02));
    strcat(file_path_02, roa_exe_path());
    strcat(file_path_02, "assets/volt_func/dev_tex_02.png");

    tex_desc_2.data = (void*)stbi_load(
      file_path_02,
      &x,
      &y,
      &n,
      0
    );
    
    tex_desc_2.width = x;
    tex_desc_2.height = y;
    tex_desc_2.format = n > 3 ? VOLT_COLOR_RGBA : VOLT_COLOR_RGB;

    volt_texture_create(ctx, &texture_1, &tex_desc_1);
    volt_texture_create(ctx, &texture_2, &tex_desc_2);

    /* vbo */
    const float verts[] = {
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
      0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    struct volt_vbo_desc vbo_desc;
    vbo_desc.data = ROA_ARR_DATA(verts);
    vbo_desc.count = ROA_ARR_COUNT(verts);

    volt_vertex_buffer_create(ctx, &vbo, &vbo_desc);

    /* ibo */
    const unsigned index[] = {
      0,1,2,
      2,3,0
    };

    struct volt_ibo_desc ibo_desc;
    ibo_desc.data = ROA_ARR_DATA(index);
    ibo_desc.count = ROA_ARR_COUNT(index);

    volt_index_buffer_create(ctx, &ibo, &ibo_desc);

    const char *vert_src = ""
      "/* Vert */\n"
      "#version 400 core\n"

      "layout(location=0) in vec3 position;\n"
      "layout(location=1) in vec3 color;\n"
      "layout(location=2) in vec2 texcoord;\n"

      "uniform mat4 model;\n"
      "uniform mat4 view;\n"
      "uniform mat4 proj;\n"

      "out vec3 Color;\n"
      "out vec2 Texcoord;\n"

      "void main() {\n"
        "Color = color;\n"
        "Texcoord = texcoord;\n"
        "mat4 wvp = proj * view * model;\n"
        "gl_Position = wvp * vec4(position, 1.0);\n"
      "}\n";

    const char* frag_src = ""
      "/* Frag */\n"
      "#version 400 core\n"
      "in vec3 Color;\n"
      "in vec2 Texcoord;\n"

      "uniform sampler2D texKitten;\n"
      "uniform sampler2D texPuppy;\n"

      "out vec4 outColor;\n"

      "void main()\n"
      "{\n"
      "outColor = mix(texture(texKitten, Texcoord), texture(texPuppy, Texcoord), 0.95);\n"
      "}\n";

    const char *stages[2];
    stages[0] = vert_src;
    stages[1] = frag_src;

    volt_shader_stage stage_types[2];
    stage_types[0] = VOLT_SHD_VERTEX;
    stage_types[1] = VOLT_SHD_FRAGMENT;

    struct volt_program_desc shd_desc;
    shd_desc.shader_stages_src = stages;
    shd_desc.shader_stages_type = stage_types;
    shd_desc.stage_count = 2;

    volt_program_create(ctx, &program, &shd_desc);

    const volt_input_attribute input_fmt[] = {
      VOLT_INPUT_FLOAT3, /* pos */
      VOLT_INPUT_FLOAT3, /* color */
      VOLT_INPUT_FLOAT2, /* tex c */
    };

    struct volt_input_desc input_desc;
    input_desc.attributes = ROA_ARR_DATA(input_fmt);
    input_desc.count      = ROA_ARR_COUNT(input_fmt);

    volt_input_create(ctx, &vert_input, &input_desc);

    volt_ctx_execute(ctx);
  }

  /* app check */
  ROA_ASSERT(vbo);
  ROA_ASSERT(ibo);
  ROA_ASSERT(vert_input);
  ROA_ASSERT(program);
  ROA_ASSERT(texture_1);
  ROA_ASSERT(texture_2);

  while (app_tick())
  {
    static int count = 0;
    ++count;

    /* create mats */
    {
      static float time = 0.1f;
      time += 0.0001f;
      float radius = 4.f;

      roa_mat4_projection(&proj, ROA_QUART_TAU * 0.75, 0.1f, 10.f, 800.f / 480.f);

      float x = roa_sin(time) * radius;
      float y = roa_cos(time) * radius;

      roa_float3 from = roa_float3_set_with_values(x, y, 1.f);
      roa_float3 at = roa_float3_fill_with_value(0.f);
      roa_float3 up = roa_float3_set_with_values(0.f, 0.f, 1.f);

      roa_mat4_lookat(&view, from, at, up);

      roa_mat4_id(&world);
    }

    volt_uniform_update(ctx, view_data, view.data);
    volt_uniform_update(ctx, proj_data, proj.data);
    volt_uniform_update(ctx, world_data, world.data);

    /* draw some stuff */
    {
      volt_renderpass_t rp;
      volt_renderpass_create(ctx, &rp);

      /* bind program */
      volt_renderpass_bind_program(rp, program);

      /* bind buffers */
      volt_renderpass_bind_vertex_buffer(rp, vbo);
      /*volt_renderpass_bind_index_buffer(rp, ibo);*/
      volt_renderpass_bind_texture_buffer(rp, texture_1, "texKitten");
      volt_renderpass_bind_texture_buffer(rp, texture_2, "texPuppy");
      volt_renderpass_bind_uniform(rp, view_data, "view");
      volt_renderpass_bind_uniform(rp, proj_data, "proj");
      volt_renderpass_bind_uniform(rp, world_data, "model");

      /* bind formatting */
      volt_renderpass_bind_input_format(rp, vert_input);
      volt_renderpass_bind_rasterizer(rp, rasterizer);

      /* draw */
      volt_renderpass_draw(rp);

      volt_renderpass_commit(ctx, &rp);
    }

    volt_ctx_execute(ctx);
  }

  volt_ctx_destroy(&ctx);

  app_destroy();

  return 0;
}


/* app details */


void
app_create()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 480, "My Title", NULL, NULL);
	glfwMakeContextCurrent(window);

  printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
}


ROA_BOOL
app_tick()
{
	glfwPollEvents();
	glfwSwapBuffers(window);

	return glfwWindowShouldClose(window) ? ROA_FALSE : ROA_TRUE;
}


void
app_destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	window = ROA_NULL;
}